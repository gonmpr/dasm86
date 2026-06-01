#include "dasm.h"


const reg_t reg_table[2][8] = {

    { REG_AL, REG_CL, REG_DL, REG_BL,
      REG_AH, REG_CH, REG_DH, REG_BH },

    { REG_AX, REG_CX, REG_DX, REG_BX,
      REG_SP, REG_BP, REG_SI, REG_DI }
};

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

  // REG/MEM + INM [ADD, SUB, CMP], all three share the same pattern, they diff each other with
  // the reg field, 111 = cmp, 101 = sub, 000 = add
  {.mask = 0b11111100, .pattern = 0b10000000, .kind = OP_INM_RM}, 

  {.mask = 0b11111111, .pattern = 0b01110101, .kind = OP_JNZ},
};


const size_t op_count = sizeof(op_set) / sizeof(op_set[0]);


const memory_operand_t eff_addr_table[] = {

  {.base = REG_BX, .index = REG_SI, .displacement = 0},
  {.base = REG_BX, .index = REG_DI, .displacement = 0},
  {.base = REG_BP, .index = REG_SI, .displacement = 0},
  {.base = REG_BP, .index = REG_DI, .displacement = 0},
  {.base = REG_SI, .index = REG_INVALID, .displacement = 0},
  {.base = REG_DI, .index = REG_INVALID, .displacement = 0},
  {.base = REG_BP, .index = REG_INVALID, .displacement = 0},
  {.base = REG_BX, .index = REG_INVALID, .displacement = 0},

};

// get the register
reg_t get_register(u8 bits, bool wide){
    if (bits > 7) {
        return REG_INVALID;
    }

    return reg_table[wide][bits];
}

// set the destiny correctly
void set_src_dst(instruction_t *ins, bool d) {
    operand_t reg = ins->operands[0];
    operand_t rm  = ins->operands[1];

    if (d) {
        ins->operands[0] = reg;
        ins->operands[1] = rm;
    } else {
        ins->operands[0] = rm;
        ins->operands[1] = reg;
    }
}



// decode opcode
opcode_t get_opcode(unsigned char byte){

  for(size_t i=0; i<op_count; i++){

    opcode_t op = op_set[i];

    if ((byte & op.mask) == op.pattern){
      return op;
    }

  }

  opcode_t invalid = {0};
  return invalid;
}







//decodes the mod reg rm structure and the displacement
// setups the operands
void decode_modrm(memory_t *mem, instruction_t *ins, bool wide){

  u8 byte2 = mem->data[mem->offset + 1];

  u8 mod = (byte2 >> 6) & 0b11;
  u8 reg = (byte2 >> 3) & 0b111;
  u8 rm  = byte2 & 0b111;


  ins->operands[0].kind = OPERAND_REGISTER;
  ins->operands[0].reg = get_register(reg, wide);

  ins->size+=2;

  //caso especial de mierda, hay displacement donde dice que no hay
  if(mod == MEM0_MOD && rm == 6){
    ins->operands[1].kind = OPERAND_ADDRESS;
    ins->operands[1].mem.base = REG_INVALID;
    ins->operands[1].mem.index = REG_INVALID;

    u8 lo = mem->data[mem->offset + 2];
    u8 hi = mem->data[mem->offset + 3];

    ins->operands[1].mem.displacement = lo | (hi << 8);

    ins->size+=2;
    return;
  }


  if (mod==REG_MOD){ //then operand[1] is also a register
    ins->operands[1].kind = OPERAND_REGISTER;
    ins->operands[1].reg = get_register(rm, wide);
    return;
  } 

  ins->operands[1].kind = OPERAND_ADDRESS;
  ins->operands[1].mem = eff_addr_table[rm];

  switch(mod){
    case MEM8_MOD:
      {
        u8 byte3 = mem->data[mem->offset + 2];
        ins->operands[1].mem.displacement = byte3;
        ins->size+=1;
        break;
      }
    case MEM16_MOD:
      {
        u8 byte3 = mem->data[mem->offset + 2];
        u8 byte4 = mem->data[mem->offset + 3];
        ins->operands[1].mem.displacement = byte3 | (byte4<<8);
        ins->size+=2;
        break;
      }
  }


}










instruction_t decode_mov(memory_t *mem, instruction_t ins){
  
  u8 byte1 = mem->data[mem->offset];

  switch(ins.opcode.pattern){

    case 0b10001000: // REG -> REG/MEM
      {
        bool w = (byte1 & 0b1) != 0;  
        bool d = (byte1 & 0b10) != 0;

        decode_modrm(mem, &ins, w);
        set_src_dst(&ins, d);
        break;
      }

    case 0b11000110: // INM -> REG/MEM MOV [], val
      
      {

        ins.opcode = get_opcode(byte1);
        bool w = (byte1 & 0b1) != 0;  
        decode_modrm(mem, &ins, w);

        u8 imm_offset = ins.size;

        u16 imm_value = mem->data[mem->offset + imm_offset];
        ins.size += 1;

        if(w){
            u16 hi = mem->data[mem->offset + imm_offset + 1];
            imm_value |= (hi << 8);
            ins.size += 1;
        }
            
        operand_t rm = ins.operands[1];

        ins.operands[0] = rm;

        ins.operands[1].kind = OPERAND_IMMEDIATE;
        ins.operands[1].immediate = imm_value;
        break;
      }

    case 0b10110000: // INM -> REG
      {
      ins.opcode = get_opcode(byte1);
      u8 reg_bits = byte1 & 0b111;
      bool w = ((byte1>>3) & 0b1) & 1;  

      ins.operands[0].kind = OPERAND_REGISTER;
      ins.operands[0].reg = get_register(reg_bits, w);

      ins.operands[1].kind = OPERAND_IMMEDIATE;
        
      u16 inm_value = mem->data[mem->offset + 1];
      ins.size += 2;

      if(w){
        u16 byte3 = mem->data[mem->offset + 2]; 
        inm_value |= (byte3<<8);
        ins.size = 3;
          }
            
        ins.operands[1].immediate = inm_value;
      break;
      }
  }
      return ins;
}
  

//
//
// // TODO 
//}
//
//instruction_t decode_add(memory_t *mem){
//
//}
//
//instruction_t decode_sub(memory_t *mem){
//
//}
//
//instruction_t decode_cmp(memory_t *mem){
//
//}
//
//instruction_t decode_jnz(memory_t *mem){
//
//}






//then decode things in base of opcode
instruction_t decode_instruction(memory_t *mem){

    instruction_t ins = {0};
    ins.opcode = get_opcode(mem->data[mem->offset]);

    switch (ins.opcode.kind) {

        case OP_INVALID:
        {
          break;
        }

        case OP_INM_RM:
        {
          //here goes a function that takes the reg field, and return the
          // op made, etc.
          break;
        }
        case OP_MOV:
        {
          ins = decode_mov(mem, ins);
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




