#ifndef CODE_H
#define CODE_H

#include <stdlib.h>
#include "memory/instruction.h"

extern inst_t program[INST_LEN];

inst_t program[INST_LEN] = {
    // add function
    {
        PUSH,
        {REG, 0, 0, (uint64_t*)&reg.rbp, NULL},
        {EMPTY, 0, 0, NULL, NULL},
        "push   \%rbp"
    },
    {
        MOV_REG_REG,
        {REG, 0, 0, (uint64_t*)&reg.rsp, NULL},
        {REG, 0, 0, (uint64_t*)&reg.rbp, NULL},
        "mov    \%rsp,\%rbp"  
    },
    {
        MOV_REG_MEM,
        {REG, 0, 0, (uint64_t*)&reg.rdi, NULL},
        {MM_IMM_REG, -0x18, 0, (uint64_t*)&reg.rbp, NULL},
        "mov    \%rdi,-0x18(\%rbp)"
    },
    {
        MOV_REG_MEM,
        {REG, 0, 0, (uint64_t*)&reg.rsi, NULL},
        {MM_IMM_REG, -0x20, 0, (uint64_t*)&reg.rbp, NULL},
        "mov    \%rsi,-0x20(\%rbp)"
    },
    {
        MOV_MEM_REG,
        {MM_IMM_REG, -0x18, 0, (uint64_t*)&reg.rbp, NULL},
        {REG, 0, 0, (uint64_t*)&reg.rdx, NULL},
        "mov    -0x18(\%rbp),\%rdx"
    },
    {
        MOV_MEM_REG,
        {MM_IMM_REG, -0x20, 0, (uint64_t*)&reg.rbp, NULL},
        {REG, 0, 0, (uint64_t*)&reg.rax, NULL},
        "mov    -0x20(\%rbp),\%rax"
    },
    {
        ADD_REG_REG,
        {REG, 0, 0, (uint64_t*)&reg.rdx, NULL},
        {REG, 0, 0, (uint64_t*)&reg.rax, NULL},
        "add    %rdx,%rax"  
    },
    {
        MOV_REG_MEM,
        {REG, 0, 0, (uint64_t*)&reg.rax, NULL},
        {MM_IMM_REG, -0x8, 0, (uint64_t*)&reg.rbp, NULL},
        "mov    \%rax,-0x8(\%rbp)"
    },
    {
        MOV_MEM_REG,
        {MM_IMM_REG, -0x8, 0, (uint64_t*)&reg.rbp, NULL},
        {REG, 0, 0, (uint64_t*)&reg.rax, NULL},
        "mov    -0x8(\%rbp),\%rax"
    },
    {
        POP,
        {REG, 0, 0, (uint64_t*)&reg.rbp, NULL},
        {EMPTY, 0, 0, NULL, NULL},
        "pop    \%rbp"
    },
    {
        RET,
        {EMPTY, 0, 0, NULL, NULL},
        {EMPTY, 0, 0, NULL, NULL}, 
        "ret"
    },

    // main function
    {
        MOV_REG_REG,
        {REG, 0, 0, (uint64_t*)&reg.rdx, NULL},
        {REG, 0, 0, (uint64_t*)&reg.rsi, NULL},
        "mov    %rdx,%rsi",
    },
    {
        MOV_REG_REG,
        {REG, 0, 0, (uint64_t*)&reg.rax, NULL},
        {REG, 0, 0, (uint64_t*)&reg.rdi, NULL},
        "mov    %rax,%rdi",
    },
    {
        // reg.rip
        CALL,
        {IMM, (uint64_t)&program[0], 0, NULL, NULL},
        {EMPTY, 0, 0, NULL, NULL}
    },
    {
        MOV_REG_MEM,
        {REG, 0, 0, (uint64_t*)&reg.rax, NULL},
        {MM_IMM_REG, -0x8, 0,(uint64_t*)&reg.rbp, NULL},
        "mov    %rax,-0x8(%rbp)"
    }
};

#endif

