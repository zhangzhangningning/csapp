#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdio.h>
#include "cpu/mmu.h"
#include "cpu/register.h"

#endif


#define INST_LEN 15

#define NUM_INSTRTYPE 30

typedef enum OD_TYPE { 
    EMPTY,
    IMM,
    REG,
    MM_IMM,
    MM_REG,
    MM_IMM_REG,
    MM_REG1_REG2,
    MM_IMM_REG1_REG2,
    MM_REG2_S,
    MM_IMM_REG2_S,
    MM_REG1_REG2_S,
    MM_IMM_REG1_REG2_S
} od_type_t;

typedef enum OP {
    MOV_REG_REG,
    MOV_REG_MEM,
    MOV_MEM_REG,
    ADD_REG_REG,
    PUSH,
    POP,
    CALL,
    RET
} op_t;

typedef struct OD {
    od_type_t od_type;

    int64_t imm;
    int64_t scal;
    uint64_t *reg1;
    uint64_t *reg2;
} od_t;

typedef struct INSTRUCT_STRUCT {
    op_t op;
    od_t src;
    od_t dst;

    char code[100];
} inst_t;

inst_t program[INST_LEN];

// pointer pointing to the function
typedef void (*handler_t)(uint64_t, uint64_t);

handler_t handler_table[NUM_INSTRTYPE];

void init_handler_table();
void instruction_cycle();

void add_reg_reg_handler(uint64_t src, uint64_t dst);

void mov_reg_reg_handler(uint64_t src, uint64_t dst);

void call_handler(uint64_t src, uint64_t dst) ;