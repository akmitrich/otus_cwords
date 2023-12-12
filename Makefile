CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c11 -g

cwords: cwords.c dict.o
	$(CC) $(CFLAGS) -o target/cwords cwords.c dict.o

dict.o: dict.h dict.c
	$(CC) $(CFLAGS) -c dict.c