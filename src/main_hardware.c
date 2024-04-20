 #include <stdio.h>
 #include <string.h>
 #include "headers/cpu.h"
 #include "headers/memory.h"
 #include "headers/common.h"

 #define MAX_NUM_INSTRUCTION_CYCLE 100

static void TestAddFunctionCallAndComputation();
static void TestString2uint();
void TestParsingOperand();
//  symbols from isa and sram
void print_register(core_t *cr);
void print_stack(core_t *cr);

static void TestString2Uint()
{
    const char*nums[12] =
    {
        "0",
        "-0",
        "0x0",
        "1234",
        "0x1234",
        "0xabcd",
        "-0xabcd",
        "-1234",
        "2147483647",
        "-2147483648",
        "0x8000000000000000",
        "0xffffffffffffffff",
    };

    for (int i = 0; i < 12; ++ i)
    {
        printf("%s => %lx\n", nums[i], string2uint(nums[i]));
    }
}

int main() {
    // TestAddFunctionCallAndComputation();
    TestParsingOperand();
    // TestString2Uint();
    return 0;
}

static void TestAddFunctionCallAndComputation() {
    ACTIVE_CORE = 0x0;
    core_t *ac = (core_t *)&cores[ACTIVE_CORE];

    ac->reg.rax = 0x12340000;
    ac->reg.rbx = 0x555555555190;
    ac->reg.rcx = 0x555555555190;
    ac->reg.rdx = 0xabcd;
    ac->reg.rsi = 0x7fffffffe0e8;
    ac->reg.rdi = 0x1;
    ac->reg.rbp = 0x7fffffffdf80;
    ac->reg.rsp = 0x7fffffffdf60;

    write64bits_dram(va2pa(0x7fffffffdf80, ac), 0, ac);
    write64bits_dram(va2pa(0x7fffffffdf78, ac), 0x0, ac);
    write64bits_dram(va2pa(0x7fffffffdf70, ac), 0xabcd, ac);
    write64bits_dram(va2pa(0x7fffffffdf68, ac), 0x12340000, ac);
    write64bits_dram(va2pa(0x7fffffffdf60, ac), 0x0, ac);

   // 2 before call
    // 3 after call before push
    // 5 after rbp
    // 13 before pop
    // 14 after pop before ret
    // 15 after ret
    char assembly[15][MAX_INSTRUCTION_CHAR] = {
        "push   %rbp",              // 0
        "mov    %rsp,%rbp",         // 1
        "mov    %rdi,-0x18(%rbp)",  // 2
        "mov    %rsi,-0x20(%rbp)",  // 3
        "mov    -0x18(%rbp),%rdx",  // 4
        "mov    -0x20(%rbp),%rax",  // 5
        "add    %rdx,%rax",         // 6
        "mov    %rax,-0x8(%rbp)",   // 7
        "mov    -0x8(%rbp),%rax",   // 8
        "pop    %rbp",              // 9
        "retq",                     // 10
        "mov    %rdx,%rsi",         // 11
        "mov    %rax,%rdi",         // 12
        "callq  0",                 // 13
        "mov    %rax,-0x8(%rbp)",   // 14
    };
    ac->rip = (uint64_t)&assembly[11];
    sprintf(assembly[13], "callq  $%p", &assembly[0]);
    
    printf("begin\n");
    int time = 0;
    while (time < 15)
    {
        instruction_cycle(ac);
        print_register(ac);
        print_stack(ac);
        time ++;
    } 

    // verfiy
    int match = 1;
    match = match && (ac->reg.rax == 0x1234abcd);
    match = match && (ac->reg.rbx == 0x555555555190);
    match = match && (ac->reg.rcx == 0x555555555190);
    match = match && (ac->reg.rdx == 0x12340000);
    match = match && (ac->reg.rsi == 0xabcd);
    match = match && (ac->reg.rdi == 0x12340000);

    if (match == 1) {
        printf("register match\n");
    } else {
        printf("register not match\n");
    }
    
    match = match && (read64bits_dram(va2pa(0x7fffffffdf80, ac), ac) == 0x0);  // rbp
    match = match && (read64bits_dram(va2pa(0x7fffffffdf78, ac), ac) == 0x1234abcd);
    match = match && (read64bits_dram(va2pa(0x7fffffffdf70, ac), ac) == 0xabcd);
    match = match && (read64bits_dram(va2pa(0x7fffffffdf68, ac), ac) == 0x12340000);  
    match = match && (read64bits_dram(va2pa(0x7fffffffdf60, ac), ac) == 0x0); // rsp

    if (match == 1) {
        printf("memory match\n");
    } else {
        printf("memory not match\n");
    }
    // gdb state ret from func    
}