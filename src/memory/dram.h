#ifndef DRAM_H
#define DRAM_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> 
#include "cpu/register.h"
#include "cpu/mmu.h"

// dynamic random access memory 动态随机存取存储器  
#define MM_LEN 1000
uint8_t mm[MM_LEN]; // physical memory

uint64_t read64bits_dram(uint64_t paddr);
void write64bits_dram(uint64_t paddr, uint64_t data);

void print_register();
void print_stack();
#endif


