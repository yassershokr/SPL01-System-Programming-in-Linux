#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

// Function to print errors and exit
void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Function to read a line from the user
char *read_line(void) {
    char *line = NULL;
    size_t bufsize = 0; 
    if (getline(&line, &bufsize, stdin) == -1) {
        free(line);
        if (feof(stdin)) exit(EXIT_SUCCESS);
        else handle_error("getline");
    }
    return line;
}

char **parse_line(char *line, int *argc) {
    int bufsize = 8;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;
    if (!tokens) handle_error("malloc");

    token = strtok(line, " \t\r\n");
    while (token != NULL) {
        tokens[position++] = strdup(token);
        if (position >= bufsize) {
            bufsize *= 2;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) handle_error("realloc");
        }
        token = strtok(NULL, " \t\r\n");
    }
    tokens[position] = NULL;
    *argc = position;
    return tokens;
}

// Built-in commands
void shell_pwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) printf("%s\n", cwd);
    else perror("getcwd");
}

void shell_cd(char **args, int argc) {
    if (argc < 2) fprintf(stderr, "cd: missing argument\n");
    else if (chdir(args[1]) != 0) perror("cd");
}

void shell_echo(char **args, int argc) {
    for (int i = 1; i < argc; i++) {
        printf("%s", args[i]);
        if (i < argc - 1) printf(" ");
    }
    printf("\n");
}

// non-built-in commands
void execute_external(char **args) {
    pid_t pid = fork();
    if (pid < 0) handle_error("fork");
    if (pid == 0) {
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void free_tokens(char **tokens, int argc) {
    for (int i = 0; i < argc; i++) free(tokens[i]);
    free(tokens);
}

int main() {
    char *line;
    char **args;
    int argc;

    while (1) {
        printf("pico-shell> ");
        line = read_line();

        args = parse_line(line, &argc);
        free(line);

        if (argc == 0) {
            free_tokens(args, argc);
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            free_tokens(args, argc);
            break;
        } else if (strcmp(args[0], "pwd") == 0) {
            shell_pwd();
        } else if (strcmp(args[0], "cd") == 0) {
            shell_cd(args, argc);
        } else if (strcmp(args[0], "echo") == 0) {
            shell_echo(args, argc);
        } else {
            execute_external(args);
        }

        free_tokens(args, argc);
    }
    return 0;
}
