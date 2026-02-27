#ifndef LILDSH_H
#define LILDSH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sysexits.h>

/*
** Utility macros:
** SPACE - All whitespace characters for tokenization
*/
#define SPACE "\t\n\v\f\r "

/*
** ANSI Color codes for terminal output formatting:
** GREEN  - Green
** CYAN   - Cyan
** RED    - Red
** RST    - Reset to default color
*/
#define GREEN "\033[1;32m"
#define CYAN "\033[1;36m"
#define RED	"\033[1;31m"
#define RST "\033[0m"

#endif
