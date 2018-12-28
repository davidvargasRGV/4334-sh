#
# Shell Makefile
#
# David Vargas
# CSCI 4334-01 Operating Systems
# Professor David Egle
# University of Texas Rio Grande Valley
#
# Programming Project One
# Due 09/30/16
#

CC = gcc
CFLAGS = -Wall -I./include/

RM = rm
RFLAGS = -f

OBJ = ./bin/main.o ./bin/console.o ./bin/parser.o ./bin/exec.o
LIB = -lreadline -lncurses

all: sh
	@echo "Built Shell"

sh: main.o console.o parser.o exec.o
	$(CC) $(CFLAGS) $(OBJ) -o ./bin/sh $(LIB)

main.o: ./src/main.c
	$(CC) $(CFLAGS) -c ./src/main.c -o ./bin/main.o

console.o: ./src/console.c
	$(CC) $(CFLAGS) -c ./src/console.c -o ./bin/console.o

parser.o: ./src/parser.c
	$(CC) $(CFLAGS) -c ./src/parser.c -o ./bin/parser.o

exec.o: ./src/exec.c
	$(CC) $(CFLAGS) -c ./src/exec.c -o ./bin/exec.o

clean:
	$(RM) $(RFLAGS) ./bin/*.o
	$(RM) $(RFLAGS) ./bin/sh
