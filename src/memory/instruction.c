#include "instruction.h"

uint64_t decode_od(od_t od) {  
    if (od.od_type == IMM) {
        return *((uint64_t*)&od.imm);
    } else if (od.od_type == REG) {
        return (uint64_t)od.reg1;
    } else
    {
        // 一定是memory
        uint64_t vaddr = 0;
        if (od.od_type == MM_IMM) {
            vaddr = od.imm;
        } else if (od.od_type == MM_REG) {
            vaddr = *(od.reg1);
        } else if (od.od_type == MM_IMM_REG) {
            vaddr = od.imm + *(od.reg1); 
        } else if (od.od_type == MM_REG1_REG2) {
            vaddr = *(od.reg1) + *(od.reg2);
        } else if (od.od_type == MM_IMM_REG1_REG2) {
            vaddr = *(od.reg1) + *(od.reg2) + od.imm;
        } else if (od.od_type == MM_REG2_S) {
            vaddr = (*(od.reg2)) * od.scal;
        } else if (od.od_type == MM_IMM_REG2_S) {
            vaddr = (*(od.reg2) *od.scal) + od.imm;
        } else if (od.od_type == MM_REG1_REG2_S) {
            vaddr = *(od.reg1) + (*(od.reg2)) * od.scal;
        } else if (od.od_type == MM_IMM_REG1_REG2_S) {
            return od.imm + *(od.reg1) + (*(od.reg2)) * od.scal ;
        }

        return va2pa(vaddr);    
    }  
}

void instruction_cycle() {
    inst_t *instr = (inst_t*)reg.rip;
    // imm: imm
    // reg: value
    // mm: physical addr
    uint64_t src = decode_od(instr->src);
    uint64_t dst = decode_od(instr->dst);

    handler_t handler = handler_table[instr->op];
    handler(src, dst);

    printf("%s\n",instr->code);
}

void init_handler_table() {
    handler_table[ADD_REG_REG] = &add_reg_reg_handler;
    handler_table[MOV_REG_REG] = &mov_reg_reg_handler;
    handler_table[CALL] = &call_handler;
}

void add_reg_reg_handler(uint64_t src, uint64_t dst) {
    *(uint64_t*)dst = *(uint64_t*)src + *(uint64_t*)dst;
    reg.rip = reg.rip + sizeof(inst_t);
}

void mov_reg_mem(uint64_t src, uint64_t dst) {
    *(uint64_t*)(dst + ((inst_t*)(uint64_t*)dst)->dst.imm) = *(uint64_t*)src;
}

void mov_reg_reg_handler(uint64_t src, uint64_t dst) {
    *(uint64_t*)dst = *(uint64_t*)src;
    reg.rip = reg.rip + sizeof(inst_t);
}

void call_handler(uint64_t src, uint64_t dst) {
    // src: imm num of called function
    reg.rsp = reg.rsp - 8;
    // write return addrss to rsp memory
    write64bits_dram(va2pa(reg.rsp), reg.rip + sizeof(inst_t));
    reg.rip = src;


}