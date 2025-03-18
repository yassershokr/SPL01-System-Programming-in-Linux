#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_VARS 100
#define MAX_CMD_LEN 1024


typedef struct {
    char *name;
    char *value;
} ShellVar;

ShellVar shell_vars[MAX_VARS];
int var_count = 0;


char *get_shell_var(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(shell_vars[i].name, name) == 0) {
            return shell_vars[i].value;
        }
    }
    return NULL;
}


void set_shell_var(const char *name, const char *value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(shell_vars[i].name, name) == 0) {
            free(shell_vars[i].value);
            shell_vars[i].value = strdup(value);
            return;
        }
    }
    if (var_count < MAX_VARS) {
        shell_vars[var_count].name = strdup(name);
        shell_vars[var_count].value = strdup(value);
        var_count++;
    } else {
        fprintf(stderr, "Max variable limit reached.\n");
    }
}


void export_var(const char *name) {
    char *value = get_shell_var(name);
    if (value) {
        setenv(name, value, 1);
    } else {
        fprintf(stderr, "export: variable '%s' not found\n", name);
    }
}


void handle_redirection(char **args, int *argc) {
    for (int i = 0; i < *argc; i++) {
        if (strcmp(args[i], "<") == 0) {
            int fd = open(args[i + 1], O_RDONLY);
            if (fd < 0) {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
            *argc = i;
        } else if (strcmp(args[i], ">") == 0) {
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Error opening output file");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
            *argc = i;
        } else if (strcmp(args[i], "2>") == 0) {
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Error opening error file");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDERR_FILENO);
            close(fd);
            args[i] = NULL;
            *argc = i;
        }
    }
}


void execute_command(char **args, int argc) {
    pid_t pid = fork();
    if (pid == 0) {
        handle_redirection(args, &argc);
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork");
    }
}


char **parse_line(char *line, int *argc) {
    char **tokens = malloc(MAX_CMD_LEN * sizeof(char *));
    char *token;
    int position = 0;

    token = strtok(line, " \t\r\n");
    while (token != NULL) {
        if (strchr(token, '=') && token[0] != '$') {
            char *name = strtok(token, "=");
            char *value = strtok(NULL, "");
            if (!value) {
                fprintf(stderr, "Invalid variable assignment\n");
            } else {
                set_shell_var(name, value);
            }
            return NULL;
        }
        if (token[0] == '$') {
            char *var_value = get_shell_var(token + 1);
            tokens[position++] = var_value ? strdup(var_value) : strdup("");
        } else {
            tokens[position++] = strdup(token);
        }
        token = strtok(NULL, " \t\r\n");
    }
    tokens[position] = NULL;
    *argc = position;
    return tokens;
}


int main() {
    char line[MAX_CMD_LEN];
    char **args;
    int argc;

    while (1) {
        printf("pico-shell> ");
        if (!fgets(line, MAX_CMD_LEN, stdin)) break;

        args = parse_line(line, &argc);
        if (!args) continue;

        if (argc == 0) {
            free(args);
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            break;
        } else if (strcmp(args[0], "export") == 0 && argc > 1) {
            export_var(args[1]);
        } else {
            execute_command(args, argc);
        }

        for (int i = 0; i < argc; i++) free(args[i]);
        free(args);
    }
    return 0;
}
