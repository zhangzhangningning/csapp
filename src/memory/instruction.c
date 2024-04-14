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
            vaddr = od.imm + *(od.reg1) + (*(od.reg2)) * od.scal ;
        }

        return vaddr;   
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
    handler_table[PUSH] = &push_handler;
    handler_table[POP] = &pop_handler;
    handler_table[MOV_REG_MEM] = &mov_reg_mem_handler;
    handler_table[MOV_MEM_REG] = &mov_mem_reg_handler;
    handler_table[RET] = &ret_handler;
}

void add_reg_reg_handler(uint64_t src, uint64_t dst) {
    *(uint64_t*)dst = *(uint64_t*)src + *(uint64_t*)dst;
    reg.rip = reg.rip + sizeof(inst_t);
}

void mov_reg_mem_handler(uint64_t src, uint64_t dst) {
    // dst mem virtual   
    write64bits_dram(
        va2pa(dst),
        *(uint64_t*)src
    );

    reg.rip = reg.rip + sizeof(inst_t);
}

void mov_mem_reg_handler(uint64_t src, uint64_t dst) { // 
    // src : mem virtual address
    // dst : reg

    *(uint64_t*)dst = read64bits_dram(va2pa(src));
    reg.rip = reg.rip + sizeof(inst_t);
}


void mov_reg_reg_handler(uint64_t src, uint64_t dst) {
    *(uint64_t*)dst = *(uint64_t*)src;
    reg.rip = reg.rip + sizeof(inst_t);
}


void push_handler(uint64_t src, uint64_t dst) {   
    reg.rsp = reg.rsp - 0x8;                            // 1）指针向下移动64位，即一个地址
    write64bits_dram(va2pa(reg.rsp), *(uint64_t*)src);  // 2）把src的内容写到rsp中
    reg.rip = reg.rip + sizeof(inst_t);
}


void pop_handler(uint64_t src, uint64_t dst) {
    // src::rbp:reg
    *(uint64_t*)src = read64bits_dram(va2pa(reg.rsp)); // 
    reg.rsp += 8;
    reg.rip = reg.rip + sizeof(inst_t);
    
}

void ret_handler(uint64_t src, uint64_t dst) {
    uint64_t ret_addr = read64bits_dram(va2pa(reg.rsp));
    reg.rip = ret_addr;
    reg.rsp += 8;
}


void call_handler(uint64_t src, uint64_t dst) {
    // src: imm num of called function
    reg.rsp = reg.rsp - 8;
    // write return addrss to rsp memory
    write64bits_dram(va2pa(reg.rsp), reg.rip + sizeof(inst_t));
    reg.rip = src;
}