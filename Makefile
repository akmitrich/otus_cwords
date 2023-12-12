CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c11 -g

cwords: cwords.c
	$(CC) $(CFLAGS) -o target/cwords cwords.c