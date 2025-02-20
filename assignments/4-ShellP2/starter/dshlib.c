#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

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
int last_status = 0;

int exec_local_cmd_loop()
{
    char cmd_line[SH_CMD_MAX];
    cmd_buff_t cmd;

    while (1)
    {
        printf("%s", SH_PROMPT);

        if (fgets(cmd_line, SH_CMD_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        cmd_line[strcspn(cmd_line, "\n")] = '\0';

        // Trim leading and trailing spaces
        char *start = cmd_line;
        while (isspace((unsigned char)*start)) start++;
        char *end = start + strlen(start) - 1;
        while (end > start && isspace((unsigned char)*end)) *end-- = '\0';

        if (strlen(start) == 0) {
            continue;
        }

        if (strcmp(start, EXIT_CMD) == 0)
        {
            break;
        }

        if (build_cmd_buff(start, &cmd) != OK)
        {
            printf("Error: failed to parse command\n");
            last_status = 1;
            continue;
        }

        Built_In_Cmds built_in_status = exec_built_in_cmd(&cmd);
        if (built_in_status == BI_EXECUTED) {
            free(cmd._cmd_buffer);
            last_status = 0; 
            continue;
        }

        last_status = exec_cmd(&cmd);
        free(cmd._cmd_buffer);
    }

    return OK;
}


int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    if (!cmd_line || strlen(cmd_line) == 0) {
        return WARN_NO_CMDS;
    }

    memset(cmd_buff, 0, sizeof(cmd_buff_t));

    cmd_buff->_cmd_buffer = strdup(cmd_line);
    if (!cmd_buff->_cmd_buffer) {
        return ERR_MEMORY;
    }

    char *ptr = cmd_buff->_cmd_buffer;
    int argc = 0;
    char *arg_start = NULL;
    bool in_quotes = false;

    while (*ptr) {
        // Skip leading spaces
        while (*ptr == ' ') {
            ptr++;
        }

        if (*ptr == '\0') {
            break;  // If end of string is reached, stop processing
        }

        // Handle quoted argument
        if (*ptr == '\"') {
            in_quotes = true;
            arg_start = ++ptr;  // Move past opening quote

            // Find closing quote
            while (*ptr && *ptr != '\"') {
                ptr++;
            }

            if (*ptr == '\"') {
                *ptr = '\0';  // Null-terminate the quoted string
                ptr++;        // Move past the closing quote
            }
        } else {  // Handle unquoted argument
            arg_start = ptr;
            while (*ptr && *ptr != ' ') {
                if (*ptr == '\"') {
                    in_quotes = true;  // Entering quoted argument
                    break;
                }
                ptr++;
            }

            if (!in_quotes && *ptr == ' ') {
                *ptr = '\0';  // Null-terminate the argument
                ptr++;
            }
        }

        // Store parsed argument
        if (arg_start && argc < CMD_ARGV_MAX - 1) {
            cmd_buff->argv[argc++] = arg_start;
        }

        in_quotes = false;  // Reset quote tracking
    }

    cmd_buff->argc = argc;
    cmd_buff->argv[argc] = NULL;  // Null-terminate argv[]

    return OK;
}

int exec_cmd(cmd_buff_t *cmd) {
    if (cmd->argc == 0 || cmd->argv[0] == NULL) {
        return WARN_NO_CMDS; // No command to execute
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork"); // Fork failed
        return ERR_EXEC_CMD;
    }

    if (pid == 0) { // Child process
        execvp(cmd->argv[0], cmd->argv);
        
        // If execvp fails, print a useful error message before exiting
        fprintf(stderr, "dsh: command not found: %s\n", cmd->argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parent process waits for the child process to finish
    int status;
    waitpid(pid, &status, WUNTRACED);

    // Return child's exit status
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    return ERR_EXEC_CMD;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (strcmp(cmd->argv[0], "echo") == 0) {
        // Check if the second argument is "$?"
        if (cmd->argc == 2 && strcmp(cmd->argv[1], "$?") == 0) {
            printf("%d\n", last_status);  // Print last exit status
            return BI_EXECUTED;
        }
    }

    if (strcmp(cmd->argv[0], "$?") == 0) {
        printf("%d\n", last_status);
        return BI_EXECUTED;
    }

    if (strcmp(cmd->argv[0], "cd") == 0) {
        if (cmd->argc == 1) {
            last_status = 0;
            return BI_EXECUTED;
        } else if (cmd->argc == 2) {
            if (chdir(cmd->argv[1]) != 0) {
                perror("cd");
                last_status = 1;
                return BI_EXECUTED;
            }
            last_status = 0;
            return BI_EXECUTED;
        } else {
            fprintf(stderr, "cd: too many arguments\n");
            last_status = 1;
            return BI_EXECUTED;
        }
    }

    return BI_NOT_BI;  // Not a built-in command
}
