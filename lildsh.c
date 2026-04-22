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
    for (char *token = strtok(line, LILDSH_SPACE); token != NULL; token = strtok(NULL, LILDSH_SPACE)) {
        tokens[position++] = token;
        if (position >= bufsize) {
            bufsize *= 2;
            tokens = lildsh_realloc(tokens, bufsize * sizeof(char *));
        }
    }

    tokens[position] = NULL;
    return tokens;
}

static char *read_line(void) {
    char cwd[BUFSIZ] = "";
    lildsh_getcwd(cwd, sizeof(cwd));

    if (isatty(fileno(stdin))) {
        char prompt[BUFSIZ + 32] = "";
        if (status == 0) {
            snprintf(prompt, sizeof(prompt), "\001" GREEN "\002%s\001" RST "\002 ~ $ ", cwd);
        } else {
            snprintf(prompt, sizeof(prompt), "\001" RED "\002%s\001" RST "\002 ~ $ ", cwd);
        }

        char *buf = readline(prompt);
        if (buf == NULL) {
            printf(RED "[EOF]\n" RST);
            return NULL;
        }

        if (*buf != '\0') {
            add_history(buf);
        }

        return buf;
    }

    char *buf = NULL;
    size_t bufsize = 0;
    lildsh_getline(&buf, &bufsize, stdin);
    return buf;
}

int main(void) {
    rl_bind_keyseq("\033[3~", rl_delete);

    char *line = NULL;
    char **args = NULL;
    while ((line = read_line()) != NULL) {
        args = splitline(line);
        if (args[0] != NULL) {
            exec(args);
        }

        free(line);
        free(args);
    }

    return EXIT_SUCCESS;
}
