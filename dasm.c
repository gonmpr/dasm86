#include "dasm.h"

const opcode_t op_set[] = {

  {.mask = 0b11111100, .pattern = 0b10001000, .kind = OP_MOV}, // REG -> REG/MEM
  {.mask = 0b11111110, .pattern = 0b11000110, .kind = OP_MOV}, // INM -> REG/MEM
  {.mask = 0b11110000, .pattern = 0b10110000, .kind = OP_MOV}, // INM -> REG

  {.mask = 0b11111100, .pattern = 0b00000000, .kind = OP_ADD}, // REG/MEM + REG
  {.mask = 0b11111110, .pattern = 0b00000100, .kind = OP_ADD}, // ACC + INM

  {.mask = 0b11111100, .pattern = 0b00101000, .kind = OP_SUB}, // REG/MEM + REG
  {.mask = 0b11111110, .pattern = 0b00101100, .kind = OP_SUB}, // ACC + INM

  {.mask = 0b11111100, .pattern = 0b00111000, .kind = OP_CMP}, // REG/MEM + REG
  {.mask = 0b11111110, .pattern = 0b00111100, .kind = OP_CMP}, // ACC + INM

  // REG/MEM + INM, all three share the same pattern, they diff each other with
  // the reg field, 111 = cmp, 101 = sub, 000 = add
  {.mask = 0b11111100, .pattern = 0b10000000, .kind = OP_INM_RM}, 

  {.mask = 0b11111111, .pattern = 0b01110101, .kind = OP_JNZ},
};


const size_t op_count = sizeof(op_set) / sizeof(op_set[0]);


//first decode opcode
opcode_t get_opcode(unsigned char byte){

  for(size_t i=0; i<insc; i++){
    opcode_t op = op_set[i];

    if ((byte & op.mask) == op.pattern){
      return op;
    }

  }

  opcode_t invalid = {0};
  return invalid;
}

//then decode things in base of opcode
instruction_t decode_instruction(memory_t *mem){

    instruction_t ins = {0};
    ins.opcode = get_opcode(mem->data[mem->offset]);

    switch (ins.opcode.kind) {

        case OP_INVALID:
        {
          return ins;
        }

        case OP_MOV:
        {
            break;
        }

        case OP_ADD:
        {
            break;
        }

        case OP_SUB:
        {
            break;
        }

        case OP_CMP:
        {
            break;
        }

        case OP_JNZ:
        {
            break;
        }
    }

    return ins;
}




