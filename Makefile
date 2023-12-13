CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c11 -g
OUT=target

cwords: cwords.c dict.o
	$(CC) $(CFLAGS) -o $(OUT)/cwords cwords.c $(OUT)/dict.o

dict.o: dict.h dict.c
	$(CC) $(CFLAGS) -o $(OUT)/dict.o -c dict.c