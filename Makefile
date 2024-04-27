CC = /usr/bin/gcc
CFLAGS = -Wall -g -O0 -Werror -std=gnu99 -Wno-unused-function


BIN_HADRWARE = ./bin/test_hardware

SRC_DIR = ./src

# debug
COMMON = $(SRC_DIR)/common/print.c $(SRC_DIR)/common/convert.c

#hardware
CPU = $(SRC_DIR)/hardware/cpu/isa.c $(SRC_DIR)/hardware/cpu/mmu.c 
MEMORY = $(SRC_DIR)/hardware/memory/dram.c 


#main
TEST_HARDWARE = $(SRC_DIR)/tests/test_hardware.c

CODE = $(COMMON) $(CPU) $(MEMORY) $(TEST_HARDWARE) 

.PHONY:hardware

hardware:
	chmod +x ./build_directory.sh
	./build_directory.sh   
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(CODE) -o $(BIN_HADRWARE)
	./$(BIN_HADRWARE)

clean:
	rm -f *.o *~$(BIN_HADRWARE)
