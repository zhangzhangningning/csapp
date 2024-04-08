

#include "memory/instruction.h"
#include "cpu/mmu.h"
#include "memory/dram.h"


int main() {
    init_handler_table();

    reg.rax = 0x12340000;
    reg.rbx = 0x555555555190;
    reg.rcx = 0x555555555190;
    reg.rdx = 0xabcd;
    reg.rsi = 0x7fffffffe0e8;
    reg.rdi = 0x1;
    reg.rbp = 0x7fffffffdf80;
    reg.rsp = 0x7fffffffdf60;

    reg.rip = (uint64_t)&program[11];

    write64bits_dram(va2pa(0x7fffffffdf80), 0x0);
    write64bits_dram(va2pa(0x7fffffffdf78), 0x0);
    write64bits_dram(va2pa(0x7fffffffdf70), 0xabcd);
    write64bits_dram(va2pa(0x7fffffffdf68), 0x12340000);
    write64bits_dram(va2pa(0x7fffffffdf60), 0x0);

    print_register();
    print_stack();
    printf("========================\n");

    // run instruciton
    for (int i = 0; i < 3; ++i) {
        instruction_cycle();
        print_register();
        print_stack();
        printf("========================\n");
    }

    // verfiy
    int match = 1;
    match = match && (reg.rax == 0x1234abcd);
    match = match && (reg.rbx == 0x555555555190);
    match = match && (reg.rcx == 0x555555555190);
    match = match && (reg.rdx == 0x12340000);
    match = match && (reg.rsi == 0xabcd);
    match = match && (reg.rdi == 0x12340000);

    if (match == 1) {
        printf("register match\n");
    } else {
        printf("register not match\n");
    }
    
    match = match && (read64bits_dram(va2pa(0x7fffffffdf80)) == 0x0);  // rbp
    match = match && (read64bits_dram(va2pa(0x7fffffffdf78)) == 0x1234abcd);
    match = match && (read64bits_dram(va2pa(0x7fffffffdf70)) == 0xabcd);
    match = match && (read64bits_dram(va2pa(0x7fffffffdf68)) == 0x12340000);  
    match = match && (read64bits_dram(va2pa(0x7fffffffdf60)) == 0x0); // rsp

    if (match == 1) {
        printf("memory match\n");
    } else {
        printf("memory not match\n");
    }

    return 0;
}