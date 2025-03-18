#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


typedef struct ShellVar {
    char *name;
    char *value;
    struct ShellVar *next;
} ShellVar;

ShellVar *var_list = NULL;


void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}


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


void set_local_var(const char *name, const char *value) {
    ShellVar *curr = var_list;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            free(curr->value);
            curr->value = strdup(value);
            return;
        }
        curr = curr->next;
    }
    ShellVar *new_var = malloc(sizeof(ShellVar));
    new_var->name = strdup(name);
    new_var->value = strdup(value);
    new_var->next = var_list;
    var_list = new_var;
}


char *get_local_var(const char *name) {
    ShellVar *curr = var_list;
    while (curr) {
        if (strcmp(curr->name, name) == 0) return curr->value;
        curr = curr->next;
    }
    return NULL;
}


void export_var(const char *name) {
    char *value = get_local_var(name);
    if (value) setenv(name, value, 1);
    else fprintf(stderr, "export: variable '%s' not found\n", name);
}


int is_valid_assignment(char *line) {
    char *equal_pos = strchr(line, '=');
    if (!equal_pos) return 0;
    if (equal_pos == line || strchr(equal_pos + 1, ' ')) return -1;
    return 1;
}


void handle_assignment(char *line) {
    char *equal_pos = strchr(line, '=');
    *equal_pos = '\0';
    char *name = line;
    char *value = equal_pos + 1;
    set_local_var(name, value);
}


void substitute_variables(char **args, int argc) {
    for (int i = 0; i < argc; i++) {
        if (args[i][0] == '$') {
            char *var_name = args[i] + 1;
            char *value = get_local_var(var_name);
            if (!value) value = getenv(var_name);
            free(args[i]);
            args[i] = strdup(value ? value : "");
        }
    }
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

// Free tokens
void free_tokens(char **tokens, int argc) {
    for (int i = 0; i < argc; i++) free(tokens[i]);
    free(tokens);
}

// Free local variables on exit
void free_local_vars() {
    ShellVar *curr = var_list;
    while (curr) {
        ShellVar *tmp = curr;
        free(curr->name);
        free(curr->value);
        curr = curr->next;
        free(tmp);
    }
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

        // Check for assignment
        if (is_valid_assignment(args[0]) == 1) {
            if (argc > 1) fprintf(stderr, "Invalid command\n");
            else handle_assignment(args[0]);
        } else if (is_valid_assignment(args[0]) == -1) {
            fprintf(stderr, "Invalid command\n");
        } else if (strcmp(args[0], "exit") == 0) {
            free_tokens(args, argc);
            break;
        } else if (strcmp(args[0], "export") == 0) {
            if (argc != 2) fprintf(stderr, "Usage: export var_name\n");
            else export_var(args[1]);
        } else {
            substitute_variables(args, argc);
            if (strcmp(args[0], "pwd") == 0) shell_pwd();
            else if (strcmp(args[0], "cd") == 0) shell_cd(args, argc);
            else if (strcmp(args[0], "echo") == 0) shell_echo(args, argc);
            else execute_external(args);
        }

        free_tokens(args, argc);
    }

    free_local_vars();
    return 0;
}
