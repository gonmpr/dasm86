#include <assert.h>
#include  "emul.h"

/* 

  *******************
  functions to define
  *******************

execute_add
execute_sub
execute_cmp
execute_jnz


*/


u8 get_reg8(cpu_t *cpu, reg_t reg){

    switch(reg)
    {
        case REG_AL: return cpu->regs.ax & 0xFF;
        case REG_AH: return cpu->regs.ax >> 8;

        case REG_BL: return cpu->regs.bx & 0xFF;
        case REG_BH: return cpu->regs.bx >> 8;

        case REG_CL: return cpu->regs.cx & 0xFF;
        case REG_CH: return cpu->regs.cx >> 8;

        case REG_DL: return cpu->regs.dx & 0xFF;
        case REG_DH: return cpu->regs.dx >> 8;

        default: return 0;
    }
}



void set_reg8(cpu_t *cpu, reg_t reg, u8 value){

    switch(reg)
    {
        case REG_AL: {
          cpu->regs.ax = (cpu->regs.ax & 0xFF00) | value;
          break;
          }
        case REG_AH: {
          cpu->regs.ax = (cpu->regs.ax & 0x00FF) | (value<<8);
          break;
          }

        case REG_BL: {
          cpu->regs.bx = (cpu->regs.bx & 0xFF00) | value;
          break;
          }
        case REG_BH: {
          cpu->regs.bx = (cpu->regs.bx & 0x00FF) | (value<<8);
          break;
          }

        case REG_CL: {
          cpu->regs.cx = (cpu->regs.cx & 0xFF00) | value;
          break;
          }
        case REG_CH: {
          cpu->regs.cx = (cpu->regs.cx & 0x00FF) | (value<<8);
          break;
          }

        case REG_DL: {
          cpu->regs.dx = (cpu->regs.dx & 0xFF00) | value;
          break;
          }
        case REG_DH: {
          cpu->regs.dx = (cpu->regs.dx & 0x00FF) | (value<<8);
          break;
          }

        default: break;
    }
}




u16 get_reg16(cpu_t *cpu, reg_t reg){

    switch(reg)
    {
        case REG_AX: return cpu->regs.ax;
        case REG_CX: return cpu->regs.cx;
        case REG_DX: return cpu->regs.dx;
        case REG_BX: return cpu->regs.bx;
        case REG_SP: return cpu->regs.sp;
        case REG_BP: return cpu->regs.bp;
        case REG_SI: return cpu->regs.si;
        case REG_DI: return cpu->regs.di;
        default: return 0;
    }
}



void set_reg16(cpu_t *cpu, reg_t reg, u16 value){

    switch(reg)
    {
        case REG_AX: {cpu->regs.ax = value; break;}
        case REG_CX: {cpu->regs.cx = value; break;}
        case REG_DX: {cpu->regs.dx = value; break;}
        case REG_BX: {cpu->regs.bx = value; break;}
        case REG_SP: {cpu->regs.sp = value; break;}
        case REG_BP: {cpu->regs.bp = value; break;}
        case REG_SI: {cpu->regs.si = value; break;}
        case REG_DI: {cpu->regs.di = value; break;}
        default: break;
    }
}



u16 effective_address(cpu_t *cpu, operand_t op){
  //just in case, im stupid
  assert(op.kind == OPERAND_ADDRESS); 

  u16 address = op.mem.displacement;

  if(op.mem.base != REG_INVALID)
      address += get_reg16(cpu, op.mem.base);

  if(op.mem.index != REG_INVALID)
      address += get_reg16(cpu, op.mem.index);

  return address;
}


// toma un operand y devuelve el valor que representa
// sea un registro, memoria, inmediato.
u16 get_operand_value(cpu_t *cpu, operand_t op){

  switch(op.kind){

    case OPERAND_REGISTER:
      {
        return op.wide
           ? get_reg16(cpu, op.reg)
           : get_reg8(cpu, op.reg);
      }
    case OPERAND_ADDRESS:
      {
        u16 addr = effective_address(cpu, op);

        if(!op.wide)
            return cpu->memory[addr];

        return cpu->memory[addr]
             | (cpu->memory[addr + 1] << 8);
      }
    case OPERAND_IMMEDIATE:
        return op.immediate;

    case OPERAND_INVALID:
    default:
        return 0;
    }
}
//toma un operando, y escribe el valor dado en el lugar
//que indique el operand
void set_operand_value(cpu_t *cpu, operand_t op, u16 value){
  switch(op.kind){

    case OPERAND_REGISTER:
      {
        op.wide ?
          set_reg16(cpu, op.reg, value):
          set_reg8(cpu, op.reg, (u8)value);
        break;
      }
    case OPERAND_ADDRESS:
      {
        u16 addr = effective_address(cpu, op);
        if(op.wide){
            cpu->memory[addr] = value & 0xFF;
            cpu->memory[addr + 1] = value >> 8;
        }
        else{
            cpu->memory[addr] = value;
        }
        break;
      }

    case OPERAND_IMMEDIATE:
    case OPERAND_INVALID:
    default:
        break;
    }
}


void execute_add(cpu_t *cpu, instruction_t ins){
  u16 dst = get_operand_value(cpu, ins.operands[0]);
  u16 src = get_operand_value(cpu, ins.operands[1]);

  u32 result = dst + src;

  u16 stored = ins.operands[0].wide ?
               (u16)result :
               (u8)result;

  cpu->flags.zf = (stored == 0);

  cpu->flags.sf = ins.operands[0].wide ?
                  ((stored & 0x8000) != 0) :
                  ((stored & 0x80) != 0);

  cpu->flags.cf = ins.operands[0].wide ?
                  (result > 0xFFFF) :
                  (result > 0xFF);


  bool dst_sign;
  bool src_sign;
  bool res_sign;

  if(ins.operands[0].wide){
      dst_sign = (dst & 0x8000) != 0;
      src_sign = (src & 0x8000) != 0;
      res_sign = (stored & 0x8000) != 0;
  }
  else{
      dst_sign = (dst & 0x80) != 0;
      src_sign = (src & 0x80) != 0;
      res_sign = (stored & 0x80) != 0;
  }

  cpu->flags.of =
      (dst_sign == src_sign) &&
      (res_sign != dst_sign);



  set_operand_value(cpu, ins.operands[0], (u16)result);
}




void execute_sub(cpu_t *cpu, instruction_t ins){
  u16 dst = get_operand_value(cpu, ins.operands[0]);
  u16 src = get_operand_value(cpu, ins.operands[1]);

  u32 result = dst - src;

  u16 stored = ins.operands[0].wide ?
               (u16)result :
               (u8)result;

  cpu->flags.zf = (stored == 0);

  cpu->flags.sf = ins.operands[0].wide ?
                  ((stored & 0x8000) != 0) :
                  ((stored & 0x80) != 0);

  cpu->flags.cf = dst < src;


  bool dst_sign;
  bool src_sign;
  bool res_sign;

  if(ins.operands[0].wide){
      dst_sign = (dst & 0x8000) != 0;
      src_sign = (src & 0x8000) != 0;
      res_sign = (stored & 0x8000) != 0;
  }
  else{
      dst_sign = (dst & 0x80) != 0;
      src_sign = (src & 0x80) != 0;
      res_sign = (stored & 0x80) != 0;
  }

  cpu->flags.of =
      (dst_sign != src_sign) &&
      (res_sign != dst_sign);

  set_operand_value(cpu, ins.operands[0], (u16)result);
}






void execute_cmp(cpu_t *cpu, instruction_t ins){
  u16 dst = get_operand_value(cpu, ins.operands[0]);
  u16 src = get_operand_value(cpu, ins.operands[1]);

  u16 result = dst - src;

  u16 stored = ins.operands[0].wide ?
               (u16)result :
               (u8)result;

  cpu->flags.zf = (stored == 0);

  cpu->flags.sf = ins.operands[0].wide ?
                  ((stored & 0x8000) != 0) :
                  ((stored & 0x80) != 0);

  cpu->flags.cf = dst < src;


  bool dst_sign;
  bool src_sign;
  bool res_sign;

  if(ins.operands[0].wide){
      dst_sign = (dst & 0x8000) != 0;
      src_sign = (src & 0x8000) != 0;
      res_sign = (stored & 0x8000) != 0;
  }
  else{
      dst_sign = (dst & 0x80) != 0;
      src_sign = (src & 0x80) != 0;
      res_sign = (stored & 0x80) != 0;
  }


  cpu->flags.of =
      (dst_sign != src_sign) &&
      (res_sign != dst_sign);

}


void execute_mov(cpu_t *cpu, instruction_t ins){
    u16 value = get_operand_value(cpu, ins.operands[1]);
    set_operand_value(cpu, ins.operands[0], value);
}



void execute_jnz(cpu_t *cpu, instruction_t ins)
{
  if(!(cpu->flags.zf))
  {
    s16 disp = (s16)(s8)ins.operands[0].immediate;
    cpu->ip += ins.size + disp;
  }
}


bool execute_instruction(cpu_t *cpu, instruction_t ins){
  switch(ins.opcode.kind){
    case OP_INVALID:
      return false;
    case OP_MOV:
      {
        execute_mov(cpu, ins);
        return true;
      }

    case OP_ADD:
      {
        execute_add(cpu, ins);
        return true;
      }
    case OP_SUB:
      {
        execute_sub(cpu, ins);
        return true;
      }
    case OP_CMP:
      {
        execute_cmp(cpu, ins);
        return true;
      }
    case OP_JNZ:
      {
        execute_jnz(cpu, ins);
        return true;
      }
    case OP_INM_RM: // just for the compiler
    default:
      break; // TODO

  }

    return true;

}



