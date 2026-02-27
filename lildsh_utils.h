#ifndef LILDSH_UTILS_H
#define LILDSH_UTILS_H

#include "lildsh.h"

pid_t lildsh_fork(void);
pid_t lildsh_wait(int *status);
void lildsh_execvp(const char *file, char *const argv[]);
int lildsh_getcwd(char *cwd, size_t size);
void lildsh_getline(char **lineptr, size_t *size, FILE *stream);
void *lildsh_malloc(size_t size);
void *lildsh_realloc(void *ptr, size_t size);

#endif
