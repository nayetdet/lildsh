#include "builtins.h"

extern int status;

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

bool lildsh_builtin(char **args) {
    if (cd(args)) {
        return true;
    }

    return false;
}
