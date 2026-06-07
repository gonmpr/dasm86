#include "text.h"
#include <stdio.h>


const char *reg_names[] = {
    [REG_AL] = "al",
    [REG_CL] = "cl",
    [REG_DL] = "dl",
    [REG_BL] = "bl",
    [REG_AH] = "ah",
    [REG_CH] = "ch",
    [REG_DH] = "dh",
    [REG_BH] = "bh",

    [REG_AX] = "ax",
    [REG_CX] = "cx",
    [REG_DX] = "dx",
    [REG_BX] = "bx",
    [REG_SP] = "sp",
    [REG_BP] = "bp",
    [REG_SI] = "si",
    [REG_DI] = "di",
};

const char *opcode_names[] = {
    [OP_INVALID] = "invalid",
    [OP_MOV]     = "mov",
    [OP_ADD]     = "add",
    [OP_SUB]     = "sub",
    [OP_CMP]     = "cmp",
    [OP_JNZ]     = "jnz",
};




void print_opcode(instruction_kind_t ins_kind) {
  printf("%s ", opcode_names[ins_kind]);

}

void print_register(reg_t reg){
  printf("%s", reg_names[reg]);
}


void print_memory(memory_operand_t mem){

    bool something_printed = false;
    printf("[");

    if(mem.base != REG_INVALID){
        printf("%s", reg_names[mem.base]);
        something_printed = true;
    }

    if(mem.index != REG_INVALID){
        printf("+%s", reg_names[mem.index]);
        something_printed = true;
    }

    if(mem.displacement){
        printf("%s%u", something_printed ? "+": "", 
               mem.displacement);
    }

    printf("]");
}


void print_operand(operand_t op)
{
    switch(op.kind)
    {
        case OPERAND_REGISTER:
          {
            print_register(op.reg);
            break;
          }

        case OPERAND_IMMEDIATE:
          {
            printf("%u", op.immediate);
            break;
          }

        case OPERAND_ADDRESS:
          {
            print_memory(op.mem);
            break;
          }

        default: 
          {
            printf("not defined\n");
          }
    }
}


void print_instruction(instruction_t ins){
    
    if(ins.opcode.kind == OP_JNZ){
      print_opcode(ins.opcode.kind);
      printf(" ");
      printf("%d", (s16)ins.operands[0].immediate);
      printf("\n");
      return;
    }

    print_opcode(ins.opcode.kind);
    print_operand(ins.operands[0]);
    printf(", ");
    print_operand(ins.operands[1]);
    printf("\n");

}




void print_u16_binary(u16 value)
{
    for(int i = 15; i >= 0; --i)
    {
        printf("%u", (value >> i) & 1);

        if(i == 8)
            printf(" ");
    }
}

void print_cpu(cpu_t *cpu)
{
    printf("\n=== CPU STATE ===\n");

    printf("IP: ");
    print_u16_binary(cpu->ip);
    printf(" (%u)\n\n", cpu->ip);

    printf("FLAGS\n");
    printf("ZF=%u SF=%u CF=%u OF=%u\n\n",
           cpu->flags.zf,
           cpu->flags.sf,
           cpu->flags.cf,
           cpu->flags.of);

    printf("REGISTERS\n");

    printf("AX: ");
    print_u16_binary(cpu->regs.ax);
    printf(" (%u)\n", cpu->regs.ax);

    printf("BX: ");
    print_u16_binary(cpu->regs.bx);
    printf(" (%u)\n", cpu->regs.bx);

    printf("CX: ");
    print_u16_binary(cpu->regs.cx);
    printf(" (%u)\n", cpu->regs.cx);

    printf("DX: ");
    print_u16_binary(cpu->regs.dx);
    printf(" (%u)\n", cpu->regs.dx);

    printf("SP: ");
    print_u16_binary(cpu->regs.sp);
    printf(" (%u)\n", cpu->regs.sp);

    printf("BP: ");
    print_u16_binary(cpu->regs.bp);
    printf(" (%u)\n", cpu->regs.bp);

    printf("SI: ");
    print_u16_binary(cpu->regs.si);
    printf(" (%u)\n", cpu->regs.si);

    printf("DI: ");
    print_u16_binary(cpu->regs.di);
    printf(" (%u)\n", cpu->regs.di);

    printf("=================\n");
}
