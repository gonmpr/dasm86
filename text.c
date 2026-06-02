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
