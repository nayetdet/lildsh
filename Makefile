.PHONY: all run clean

NAME = lildsh.out
SRCS = lildsh.c lildsh_utils.c builtins.c
CC = gcc
CFLAGS = -Wall -Wextra -g
LDLIBS = -lreadline

$(NAME): $(SRCS) lildsh.h lildsh_utils.h builtins.h
	$(CC) $(CFLAGS) $(SRCS) $(LDLIBS) -o $(NAME)

all: run clean

run: $(NAME)
	./$(NAME)

clean:
	rm -f $(NAME)
