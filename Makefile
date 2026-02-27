.PHONY: all run clean

NAME = lildsh.out
SRCS = lildsh.c lildsh_utils.c
CC = gcc
CFLAGS = -Wall -Wextra -g

$(NAME):
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

all: run clean

run: $(NAME)
	./$(NAME)

clean:
	rm -f $(NAME)
