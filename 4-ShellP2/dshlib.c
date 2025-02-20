#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"
#include "errno.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

int exec_local_cmd_loop() {
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));  // Allocate memory for input buffer
    if (!cmd_buff) {
        perror("Memory allocation failed");
        return ERR_MEMORY;
    }

    cmd_buff_t cmd;
    int exit_number = 0;

    while (1) {
        printf("%s", SH_PROMPT);  // Print the shell prompt

        // Read user input
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break; 
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';  // Remove trailing newline

        if (strlen(cmd_buff) == 0) {
            printf(CMD_WARN_NO_CMD);
            exit_number = WARN_NO_CMDS;
            continue;
        }

        if (strcmp(cmd_buff, EXIT_CMD) == 0) {
            free(cmd_buff);
            exit(OK); 
        }

        cmd.argc = 0;
        cmd._cmd_buffer = cmd_buff;
        char *current = cmd_buff;
        bool in_quotes = false;
        char *arg_start = NULL;

        while (*current) {
            if (*current == '"') {
                in_quotes = !in_quotes;
                if (!in_quotes) {
                    *current = '\0'; 
                    cmd.argv[cmd.argc++] = arg_start;
                    arg_start = NULL;
                } else {
                    arg_start = current + 1;
                }
            } else if (!in_quotes && (*current == SPACE_CHAR)) {
                *current = '\0';
                if (arg_start) {
                    cmd.argv[cmd.argc++] = arg_start;
                    arg_start = NULL;
                }
            } else if (!arg_start) {
                arg_start = current;  
            }
            current++;
        }

        if (arg_start) {
            cmd.argv[cmd.argc++] = arg_start;
        }
        cmd.argv[cmd.argc] = NULL;


        if (cmd.argc == 0) {
            printf(CMD_WARN_NO_CMD);
            exit_number = WARN_NO_CMDS;
            continue;
        }

        if (strcmp(cmd.argv[0], "cd") == 0) {
            if (cmd.argc == 1) {
                continue;
            } else if (chdir(cmd.argv[1]) != 0) {
                printf("cd failed");
                exit_number = ENOTDIR;
            } else if (cmd.argc > 2){
                printf("too many arguments");
                exit_number = ERR_TOO_MANY_COMMANDS;
            }
            continue;
        }
        
        int f_result, c_results;

        f_result = fork();
        if (f_result < 0) {
            exit_number = errno;
            exit(errno);
        }

        if (f_result == 0) { 
            int rc = execvp(cmd.argv[0], cmd.argv);
            if (rc < 0){
                if (errno == ENOENT) {
                    printf("Command not found\n");
                } else if (errno == EACCES) {
                    printf("Permission denied\n");
                } else if (errno == ENOTDIR) {
                    printf("Not a directory\n");
                } else if (errno == ENOEXEC) {
                    printf("Cannot execute invalid format\n");
                }
                else {
                    printf("Failed to execute fommand\n");
                }
                exit(errno); 
            }
        } else {
            wait(&c_results);
        }

        exit_number = WEXITSTATUS(c_results);
        continue;
    }

    free(cmd_buff);
    return exit_number;
}

