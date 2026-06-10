CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c99

dasm86:
	$(CC) $(CFLAGS) *.c -o dasm86
