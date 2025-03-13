#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

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
int build_cmd_list(char *cmd_line, command_list_t *clist) {
    char temp_cmd_line[SH_CMD_MAX];
    strncpy(temp_cmd_line, cmd_line, SH_CMD_MAX - 1);
    temp_cmd_line[SH_CMD_MAX - 1] = '\0';

    char *single_cmd = strtok(temp_cmd_line, PIPE_STRING);
    int cmd_count = 0;

    memset(clist, 0, sizeof(command_list_t));

    while (single_cmd != NULL) {
        while (*single_cmd == SPACE_CHAR) {
            single_cmd++;
        }

        char *end = single_cmd + strlen(single_cmd) - 1;
        while (end > single_cmd && *end == SPACE_CHAR) {
            end--;
        }
        *(end + 1) = '\0';
        

        if (cmd_count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        if (strlen(single_cmd) >= EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        


        cmd_buff_t *cmd_buff = &clist->commands[cmd_count];
        cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
        if (cmd_buff->_cmd_buffer == NULL) {
            return ERR_MEMORY;
        }

        memset(cmd_buff, 0, sizeof(cmd_buff_t));


        if (build_cmd_buff(single_cmd, cmd_buff) != OK) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        cmd_count++;
        single_cmd = strtok(NULL, PIPE_STRING);
    }

    clist->num = cmd_count;
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = strdup(cmd_line);
    char *current = cmd_buff->_cmd_buffer;
    char *arg_start = NULL;
    int in_quotes = 0;

    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));

    while (*current) {
        if (*current == '"') {
            in_quotes = !in_quotes;
            if (!in_quotes) {
                *current = '\0';
                if (arg_start) {
                    cmd_buff->argv[cmd_buff->argc++] = arg_start;
                    arg_start = NULL;
                }
            } else {
                arg_start = current + 1;
            }
        } else if (!in_quotes && (*current == SPACE_CHAR)) {
            *current = '\0';
            if (arg_start) {
                cmd_buff->argv[cmd_buff->argc++] = arg_start;
                arg_start = NULL;
            }
        } else if (!arg_start) {
            arg_start = current;
        }

        current++;
    }

    if (arg_start) {
        cmd_buff->argv[cmd_buff->argc++] = arg_start;
    }

    cmd_buff->argv[cmd_buff->argc] = NULL;

    return OK;
}


int exec_cmd(cmd_buff_t *cmd) {
    if (exec_built_in_cmd(cmd) == BI_EXECUTED) {
        return OK;
    } 

    int f_result, c_results;
    f_result = fork();

    if (f_result < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (f_result == 0) {
        cmd->argv[cmd->argc] = NULL;
        execvp(cmd->argv[0], cmd->argv);
        return ERR_EXEC_CMD;
    } else {
        wait(&c_results);
    }

    return OK;
}


Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    Built_In_Cmds cmd_match = match_command(cmd->argv[0]);

    if (cmd_match == BI_CMD_EXIT) {
        exit(0);
    } else if (cmd_match == BI_CMD_CD) {
        if (cmd->argv[1] == NULL || chdir(cmd->argv[1]) != 0) {
            printf("Command to execute: %s\n", cmd -> argv[1]);
            printf("cd failed\n");
            return ERR_EXEC_CMD;
        }
        return BI_EXECUTED;
    } else if (cmd_match == BI_CMD_DRAGON) {
        return BI_EXECUTED;
    }
    return BI_NOT_BI;
}

Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    } else if (strcmp(input, "exit") == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(input, "dragon") == 0) {
        return BI_CMD_DRAGON;
    } else {
        return BI_NOT_BI;
    }
}

int execute_pipeline(command_list_t *clist) {
    int pipe_fds[2 * (clist->num - 1)];
    pid_t pids[clist->num];

    for (int i = 0; i < clist->num - 1; i++) {
        if (pipe(pipe_fds + 2 * i) == -1) {
            perror("pipe");
            return -1;

        }
    }



    for (int i = 0; i < clist->num; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            return ERR_TOO_MANY_COMMANDS;
        }

        if (pids[i] == 0) {
            if (i > 0) { 
                dup2(pipe_fds[2 * (i - 1)], STDIN_FILENO);
            }
            if (i < clist->num - 1) {
                dup2(pipe_fds[2 * i + 1], STDOUT_FILENO);
            }

            for (int j = 0; j < 2 * (clist->num - 1); j++) {
                close(pipe_fds[j]);
            }

            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            return ERR_EXEC_CMD;
        }
    }

    for (int i = 0; i < 2 * (clist->num - 1); i++) {
        close(pipe_fds[i]);
    }

    for (int i = 0; i < clist->num; i++) {
        int status;
        waitpid(pids[i], &status, 0);
    }

    return OK;
}


int exec_local_cmd_loop() {
    int exit_num;
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
    if (!cmd_buff) {
        fprintf(stderr, "Memory allocation failed\n");
        return ERR_MEMORY;
    }

    command_list_t clist;

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // Check if the input is only whitespace
        if (strspn(cmd_buff, " \t") == strlen(cmd_buff)) {
            printf("%s\n", CMD_WARN_NO_CMD);  // Handle empty or whitespace-only input
            exit_num = WARN_NO_CMDS;
            continue;
        }

        if (cmd_buff[strlen(cmd_buff) - 1] == '|') {
            printf("Command cannot end with Pipe\n");
            exit_num = 2;
            continue;
        }

        int build_status = build_cmd_list(cmd_buff, &clist);
        if (build_status != OK) {
            if (build_status == ERR_TOO_MANY_COMMANDS) {
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
                return ERR_TOO_MANY_COMMANDS;
            } else if (build_status == ERR_MEMORY) {
                printf("Memory allocation error\n");
                free(cmd_buff);
                return ERR_MEMORY;
            }
            continue;
        }

        if (clist.num == 1) {
            exit_num = exec_cmd(&clist.commands[0]);
        } else {
            exit_num = execute_pipeline(&clist);
        }

        for (int i = 0; i < clist.num; i++) {
            free(clist.commands[i]._cmd_buffer);
        }
        memset(&clist, 0, sizeof(command_list_t));  
    }

    free(cmd_buff);
    return exit_num;
}
