// 内存管理单元 memory management unit  phsyical address -> virtual address
#ifndef MMU_H
#define MMU_H

#include <stdint.h>
#include "cpu/mmu.h"
#include "memory/dram.h"

uint64_t va2pa(uint64_t va);

#endif
