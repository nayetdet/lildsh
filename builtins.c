#include "builtins.h"

extern int status;

static bool exit_shell(char **args) {
    if (strcmp(args[0], "exit") != 0) {
        return false;
    }

    if (args[1] == NULL) {
        exit(status);
    }

    if (args[2] != NULL) {
        fprintf(stderr, RED "exit failed: too many arguments" RST "\n");
        status = EXIT_FAILURE;
        return true;
    }

    errno = 0;
    char *end = NULL;
    long exit_status = strtol(args[1], &end, 10);
    if (errno != 0 || end == args[1] || *end != '\0') {
        fprintf(stderr, RED "exit failed: numeric argument required" RST "\n");
        exit(EX_USAGE);
    }

    exit((unsigned char)exit_status);
}

static bool cd(char **args) {
    if (strcmp(args[0], "cd") != 0) {
        return false;
    }

    const char *path = args[1];
    if (path == NULL || strcmp(path, "~") == 0) {
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, RED "cd failed: HOME not set" RST "\n");
            status = EXIT_FAILURE;
            return true;
        }
    } else if (args[2] != NULL) {
        fprintf(stderr, RED "cd failed: too many arguments" RST "\n");
        status = EXIT_FAILURE;
        return true;
    }

    if (chdir(path) == -1) {
        perror(RED "cd failed" RST);
        status = EXIT_FAILURE;
    } else {
        status = EXIT_SUCCESS;
    }

    return true;
}

static bool help(char **args) {
    if (strcmp(args[0], "help") != 0) {
        return false;
    }

    if (args[1] != NULL) {
        fprintf(stderr, RED "help failed: too many arguments" RST "\n");
        status = EXIT_FAILURE;
        return true;
    }

    printf(CYAN "lildsh builtins" RST "\n");
    printf("  cd [dir]       change current directory. Defaults to HOME.\n");
    printf("  exit [status]  exit the shell. Defaults to last command status.\n");
    printf("  help           show this help message.\n");
    printf("\nExternal commands are searched in PATH.\n");

    status = EXIT_SUCCESS;
    return true;
}

bool lildsh_builtin(char **args) {
    if (exit_shell(args)) {
        return true;
    }

    if (cd(args)) {
        return true;
    }

    if (help(args)) {
        return true;
    }

    return false;
}
