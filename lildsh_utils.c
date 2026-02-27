#include "lildsh_utils.h"

pid_t lildsh_fork(void) {
    pid_t pid = fork();
    if (pid < 0) {
		perror(RED "fork failed" RST);
		exit(EX_OSERR);
    }

    return pid;
}

pid_t lildsh_wait(int *status) {
    if (status == NULL) {
        fprintf(stderr, RED "wait failed: invalid arguments\n" RST);
        return -1;
    }

    pid_t result = wait(status);
    if (result == -1) {
        perror(RED "wait failed" RST);
        return -1;
    }

    if (WIFEXITED(*status)) {
        *status = WEXITSTATUS(*status); 
    } else if (WIFSIGNALED(*status)) {
        *status = WTERMSIG(*status) + 128;
    }

    return result;
}

void lildsh_execvp(const char *file, char *const argv[]) {
    if (file == NULL || argv == NULL) {
        fprintf(stderr, RED "execvp failed: invalid arguments\n" RST);
        exit(EXIT_FAILURE);
    }

	if (execvp(file, argv) == -1) {
		perror(RED "execvp failed" RST);
		exit(EX_UNAVAILABLE);
	}
}

int lildsh_getcwd(char *cwd, size_t size) {
    if (cwd == NULL || size == 0) {
        fprintf(stderr, RED "getcwd failed: invalid arguments\n" RST);
        return -1;
    }

    if (getcwd(cwd, size) == NULL) {
        perror(RED "getcwd failed" RST);
        return -1;
    }

    return 0;
}

void lildsh_getline(char **lineptr, size_t *size, FILE *stream) {
    if (lineptr == NULL || size == NULL || stream == NULL) {
        fprintf(stderr, RED "getline failed: invalid arguments\n" RST);
        return;
    }

    if (getline(lineptr, size, stream) == -1) {
        if (*lineptr) {
            free(*lineptr);
            *lineptr = NULL;
        }

        if (feof(stream)) {
            if (isatty(fileno(stream))) {
                printf(RED "[EOF]\n" RST);
            }
        } else {
            perror(RED "getline failed" RST);
        }
    }
}

void *lildsh_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    void *ptr = malloc(size);
    if (ptr == NULL) {
        perror(RED "malloc failed" RST);
        free(ptr);
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *lildsh_realloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL && size != 0) {
        perror(RED "realloc failed" RST);
        free(ptr);
        exit(EXIT_FAILURE);
    }

    return new_ptr;
}
