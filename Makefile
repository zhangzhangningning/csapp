CC = /usr/bin/gcc
CFLAGS = -Wall -g -O2 -Werror -std=gnu99

EXE = program

SRC = ./src

CODE = ./src/memory/instruction.c ./src/disk/code.c ./src/memory/dram.c ./src/cpu/mmu.c ./src/main.c 

.PYTHON: program

main:
	$(CC) $(CFLAGS) -I$(SRC) $(CODE) -o $(EXE)

run:
	./$(EXE)
