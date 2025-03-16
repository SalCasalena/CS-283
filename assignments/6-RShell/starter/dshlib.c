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
int last_status = 0;

int exec_local_cmd_loop()
{
    char cmd_line[SH_CMD_MAX];
    command_list_t cmd_list;

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

        // Parse input into multiple commands (split by pipes)
        int parse_status = build_cmd_list(start, &cmd_list);
        if (parse_status == ERR_TOO_MANY_COMMANDS)
        {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            last_status = 1;
            continue;
        }
        else if (parse_status != OK)
        {
            printf("Error: failed to parse command list\n");
            last_status = 1;
            continue;
        }

        // **Check if the first command is a built-in command**
        if (cmd_list.num == 1)
        {
            Built_In_Cmds bi_status = exec_built_in_cmd(&cmd_list.commands[0]);
            if (bi_status == BI_EXECUTED) 
            {
                free_cmd_list(&cmd_list);  // Free memory before skipping pipeline execution
                continue;
            }
        }

        // Execute the command(s) with pipeline support
        last_status = execute_pipeline(&cmd_list);

        // Free allocated memory for command list
        free_cmd_list(&cmd_list);
    }

    return OK;
}


int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (!cmd_buff) {
        return OK;
    }

    if (cmd_buff->_cmd_buffer) {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }

    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));

    return OK;
}

int free_cmd_list(command_list_t *cmd_lst) {
    // Implement the cleanup logic for command_list_t
    for (int i = 0; i < cmd_lst->num; i++) {
        free_cmd_buff(&cmd_lst->commands[i]);  // Free each individual command buffer
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

    while (*ptr) {
        while (*ptr == ' ') ptr++;

        if (*ptr == '<') {  // Input redirection
            ptr++;
            while (*ptr == ' ') ptr++;
            cmd_buff->input_file = ptr;
            while (*ptr && *ptr != ' ') ptr++;
            *ptr = '\0';
            ptr++;
            continue;
        }

        if (*ptr == '>') {  // Output redirection
            ptr++;
            cmd_buff->append_mode = (*ptr == '>');  // Check if it's `>>`
            if (cmd_buff->append_mode) ptr++;
            while (*ptr == ' ') ptr++;
            cmd_buff->output_file = ptr;
            while (*ptr && *ptr != ' ') ptr++;
            *ptr = '\0';
            ptr++;
            continue;
        }

        cmd_buff->argv[argc++] = ptr;
        while (*ptr && *ptr != ' ') ptr++;
        if (*ptr) *ptr++ = '\0';
    }

    cmd_buff->argc = argc;
    cmd_buff->argv[argc] = NULL;
    return OK;
}

int exec_cmd(cmd_buff_t *cmd) {
    if (cmd->argc == 0 || cmd->argv[0] == NULL) {
        return WARN_NO_CMDS;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return ERR_EXEC_CMD;
    }

    if (pid == 0) { // Child process
        int in_fd = -1, out_fd = -1;

        // **Handle input redirection (`<`)**
        if (cmd->input_file) {
            in_fd = open(cmd->input_file, O_RDONLY);
            if (in_fd == -1) {
                fprintf(stderr, "rsh: cannot open input file: %s\n", cmd->input_file);
                exit(EXIT_FAILURE);
            }
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }

        // **Handle output redirection (`>` and `>>`)**
        if (cmd->output_file) {
            int flags = O_WRONLY | O_CREAT;
            if (cmd->append_mode) {
                flags |= O_APPEND;  // Append mode (`>>`)
            } else {
                flags |= O_TRUNC;   // Overwrite mode (`>`)
            }
            out_fd = open(cmd->output_file, flags, 0644);
            if (out_fd == -1) {
                fprintf(stderr, "rsh: cannot open output file: %s\n", cmd->output_file);
                exit(EXIT_FAILURE);
            }
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }

        execvp(cmd->argv[0], cmd->argv);
        fprintf(stderr, "rsh: command not found: %s\n", cmd->argv[0]);
        exit(EXIT_FAILURE);
    }

    int status;
    waitpid(pid, &status, WUNTRACED);
    return WIFEXITED(status) ? WEXITSTATUS(status) : ERR_EXEC_CMD;
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

int execute_pipeline(command_list_t *clist) {
    if (clist->num == 0) {
        return WARN_NO_CMDS;
    }

    int pipes[CMD_MAX - 1][2]; // Pipes between commands
    pid_t pids[CMD_MAX];
    int status;

    // Create pipes (one less than the number of commands)
    for (int i = 0; i < clist->num - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return ERR_EXEC_CMD;
        }
    }

    // Fork and execute commands in the pipeline
    for (int i = 0; i < clist->num; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork");
            return ERR_EXEC_CMD;
        }

        if (pids[i] == 0) {  // Child process
            int in_fd = -1, out_fd = -1;

            // **Input redirection (`<` only for first command)**
            if (i == 0 && clist->commands[i].input_file) {
                in_fd = open(clist->commands[i].input_file, O_RDONLY);
                if (in_fd == -1) {
                    fprintf(stderr, "rsh: cannot open input file: %s\n", clist->commands[i].input_file);
                    exit(EXIT_FAILURE);
                }
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }

            // **Output redirection (`>` or `>>` only for last command)**
            if (i == clist->num - 1 && clist->commands[i].output_file) {
                int flags = O_WRONLY | O_CREAT;
                if (clist->commands[i].append_mode) {
                    flags |= O_APPEND;  // Append mode (`>>`)
                } else {
                    flags |= O_TRUNC;   // Overwrite mode (`>`)
                }
                out_fd = open(clist->commands[i].output_file, flags, 0644);
                if (out_fd == -1) {
                    fprintf(stderr, "rsh: cannot open output file: %s\n", clist->commands[i].output_file);
                    exit(EXIT_FAILURE);
                }
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }

            // **Pipe Redirections**
            if (i > 0) {  // Not the first command: Read from previous pipe
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < clist->num - 1) {  // Not the last command: Write to next pipe
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipes
            for (int j = 0; j < clist->num - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Execute the command
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            fprintf(stderr, "rsh: command not found: %s\n", clist->commands[i].argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Close all pipes in parent
    for (int i = 0; i < clist->num - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes
    for (int i = 0; i < clist->num; i++) {
        waitpid(pids[i], &status, 0);
    }

    return WIFEXITED(status) ? WEXITSTATUS(status) : ERR_EXEC_CMD;
}


int build_cmd_list(char *cmd_line, command_list_t *clist) {
    if (!cmd_line || strlen(cmd_line) == 0) {
        return WARN_NO_CMDS;
    }

    memset(clist, 0, sizeof(command_list_t));

    char *cmd_copy = strdup(cmd_line);
    if (!cmd_copy) {
        return ERR_MEMORY;
    }

    char *token = strtok(cmd_copy, "|");

    // If there's only a pipe with no command, return an error
    if (token == NULL || strlen(token) == 0) {
        free(cmd_copy);
        return ERR_CMD_ARGS_BAD; 
    }

    while (token && clist->num < CMD_MAX) {
        while (*token == ' ') token++;

        if (*token == '\0') {
            free(cmd_copy);
            return ERR_CMD_ARGS_BAD;
        }

        if (build_cmd_buff(token, &clist->commands[clist->num]) != OK) {
            free(cmd_copy);
            return ERR_CMD_ARGS_BAD;
        }

        clist->num++;
        token = strtok(NULL, "|");
    }

    free(cmd_copy);

    // If the last character in the command line is a pipe, it's an invalid command
    if (cmd_line[strlen(cmd_line) - 1] == '|') {
        return ERR_CMD_ARGS_BAD;
    }

    return (clist->num > 0) ? OK : WARN_NO_CMDS;
}
