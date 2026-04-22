#include "builtins.h"
#include "lildsh.h"
#include "lildsh_utils.h"

int status = 0;

static void exec(char **args) {
    if (lildsh_builtin(args)) {
        return;
    }

    if (lildsh_fork() == 0) {
        lildsh_execvp(args[0], args);
    } else {
        if (lildsh_wait(&status) == -1) {
            status = EXIT_FAILURE;
        }
    }
}

static char **splitline(char *line) {
    size_t position = 0;
    size_t bufsize = BUFSIZ;
    char **tokens = lildsh_malloc(bufsize * sizeof(char *));
    for (char *token = strtok(line, SPACE); token != NULL; token = strtok(NULL, SPACE)) {
        tokens[position++] = token;
        if (position >= bufsize) {
            bufsize *= 2;
            tokens = lildsh_realloc(tokens, bufsize * sizeof(char *));
        }
    }

    tokens[position] = NULL;
    return tokens;
}

static char *readline(void) {
    char cwd[BUFSIZ] = "";
    lildsh_getcwd(cwd, sizeof(cwd));
    if (isatty(fileno(stdin))) {
        if (status == 0) {
            printf(GREEN "%s" RST " ~ $ ", cwd);
        } else {
            printf(RED "%s" RST " ~ $ ", cwd);
        }
    }
    
    char *buf = NULL;
    size_t bufsize = 0;
    lildsh_getline(&buf, &bufsize, stdin);
    return buf;
}

int main(void) {
    char *line = NULL;
    char **args = NULL;
    while ((line = readline()) != NULL) {
        args = splitline(line);
        if (args[0] != NULL) {
            exec(args);
        }

        free(line);
        free(args);
    }

    return EXIT_SUCCESS;
}
