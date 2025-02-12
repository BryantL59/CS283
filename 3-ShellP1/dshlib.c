#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    memset(clist, 0, sizeof(command_list_t));

    char *single_cmd = strtok(cmd_line, PIPE_STRING);
    int cmd_count = 0;

    while (single_cmd != NULL) {
        // Trim leading spaces
        while (*single_cmd == SPACE_CHAR) {
            single_cmd++;
        }

        // Trim trailing spaces
        char *end = single_cmd + strlen(single_cmd) - 1;
        while (end > single_cmd && *end == SPACE_CHAR) {
            end--;
        }
        *(end + 1) = '\0';

        if (cmd_count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        char *space = strchr(single_cmd, SPACE_CHAR);

        if (space != NULL) { //checks for the first space char
            *space = '\0'; 
            space++;       

            while (*space == SPACE_CHAR) {
                space++;
            }
        }

        if (strlen(single_cmd) >= EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        strncpy(clist->commands[cmd_count].exe, single_cmd, EXE_MAX - 1); //copies information about command 
        if (space != NULL && *space != '\0') {
            if (strlen(space) >= ARG_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            strncpy(clist->commands[cmd_count].args, space, ARG_MAX - 1);
        }

        cmd_count++;
        single_cmd = strtok(NULL, PIPE_STRING);
    }

    clist->num = cmd_count;
    return OK;
}

