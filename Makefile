CC = /usr/bin/gcc
CFLAGS = -Wall -g -O2 -Werror -std=gnu99 -Wno-unused-function

EXE_HADRWARE = exe_hardware

SRC_DIR = ./src

# debug
COMMON = $(SRC_DIR)/common/print.c $(SRC_DIR)/common/convert.c

#hardware
CPU = $(SRC_DIR)/hardware/cpu/isa.c $(SRC_DIR)/hardware/cpu/mmu.c 
MEMORY = $(SRC_DIR)/hardware/memory/dram.c 


#main
MAIN = $(SRC_DIR)/main_hardware.c

CODE = $(COMMON) $(CPU) $(MEMORY) $(MAIN) 

.PHONY:hardware

hardware:
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(CODE) -o $(EXE_HADRWARE)
	./$(EXE_HADRWARE)

clean:
	rm -f *.o *~$(EXE_HADRWARE)
