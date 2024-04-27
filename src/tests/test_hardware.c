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
void TestParsingInstruction();
static void TestSumRecursiveCondition();

int main() {
    TestAddFunctionCallAndComputation();
    // TestParsingOperand();
    // TestParsingInstruction();
    // TestString2Uint();
    TestSumRecursiveCondition();
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
        "callq  0x00400000",        // 13
        "mov    %rax,-0x8(%rbp)",   // 14
    };

    for (int i = 0; i < 15; ++ i)
    {
        writeinst_dram(va2pa(i * 0x40 + 0x00400000, ac), assembly[i], ac);
    }
    ac->rip = MAX_INSTRUCTION_CHAR * sizeof(char) * 11 + 0x00400000;

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

static void TestSumRecursiveCondition()
{
    ACTIVE_CORE = 0x0;
    core_t *cr = (core_t *)&cores[ACTIVE_CORE];

    // init state
    cr->reg.rax = 0x8000630;
    cr->reg.rbx = 0x0;
    cr->reg.rcx = 0x8000650;
    cr->reg.rdx = 0x7ffffffee328;
    cr->reg.rsi = 0x7ffffffee318;
    cr->reg.rdi = 0x1;
    cr->reg.rbp = 0x7ffffffee230;
    cr->reg.rsp = 0x7ffffffee220;

    cr->flags.__flag_values = 0;

    write64bits_dram(va2pa(0x7ffffffee230, cr), 0x0000000008000650, cr);    // rbp
    write64bits_dram(va2pa(0x7ffffffee228, cr), 0x0000000000000000, cr);
    write64bits_dram(va2pa(0x7ffffffee220, cr), 0x00007ffffffee310, cr);    // rsp

    char assembly[19][MAX_INSTRUCTION_CHAR] = {
        "push   %rbp",              // 0
        "mov    %rsp,%rbp",         // 1
        "sub    $0x10,%rsp",        // 2
        "mov    %rdi,-0x8(%rbp)",   // 3
        "cmpq   $0x0,-0x8(%rbp)",   // 4
        "jne    0x400200",          // 5: jump to 8
        "mov    $0x0,%eax",         // 6
        "jmp    0x400380",          // 7: jump to 14
        "mov    -0x8(%rbp),%rax",   // 8
        "sub    $0x1,%rax",         // 9
        "mov    %rax,%rdi",         // 10
        "callq  0x00400000",        // 11
        "mov    -0x8(%rbp),%rdx",   // 12
        "add    %rdx,%rax",         // 13
        "leaveq ",                  // 14
        "retq   ",                  // 15
        "mov    $0x3,%edi",         // 16
        "callq  0x00400000",        // 17
        "mov    %rax,-0x8(%rbp)",   // 18
    };

    // copy to physical memory
    for (int i = 0; i < 19; ++ i)
    {
        writeinst_dram(va2pa(i * 0x40 + 0x00400000, cr), assembly[i], cr);
    }
    cr->rip = MAX_INSTRUCTION_CHAR * sizeof(char) * 16 + 0x00400000;

    printf("begin\n");
    int time = 0;
    while ((cr->rip <= 18 * 0x40 + 0x00400000) &&
           time < MAX_NUM_INSTRUCTION_CYCLE)
    {
        instruction_cycle(cr);
        print_register(cr);
        print_stack(cr);
        time ++;
    } 

    // gdb state ret from func
    int match = 1;
    match = match && cr->reg.rax == 0x6;
    match = match && cr->reg.rbx == 0x0;
    match = match && cr->reg.rcx == 0x8000650;
    match = match && cr->reg.rdx == 0x3;
    match = match && cr->reg.rsi == 0x7ffffffee318;
    match = match && cr->reg.rdi == 0x0;
    match = match && cr->reg.rbp == 0x7ffffffee230;
    match = match && cr->reg.rsp == 0x7ffffffee220;
    
    if (match)
    {
        printf("register match\n");
    }
    else
    {
        printf("register mismatch\n");
    }

    match = match && (read64bits_dram(va2pa(0x7ffffffee230, cr), cr) == 0x0000000008000650); // rbp
    match = match && (read64bits_dram(va2pa(0x7ffffffee228, cr), cr) == 0x0000000000000006);
    match = match && (read64bits_dram(va2pa(0x7ffffffee220, cr), cr) == 0x00007ffffffee310); // rsp

    if (match)
    {
        printf("memory match\n");
    }
    else
    {
        printf("memory mismatch\n");
    }
}