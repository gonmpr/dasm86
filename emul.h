#include <stdint.h>
#include <stdbool.h>
#include "dasm.h"


typedef struct {

  struct {
      u16 ax;
      u16 cx;
      u16 dx;
      u16 bx;

      u16 sp;
      u16 bp;
      u16 si;
      u16 di;
  } regs;


  struct {
      bool zf; //zero
      bool sf; //sign
      bool cf; //carry
      bool of; //overflow
  } flags;


  u16 ip;

  u8 memory[65536]; // 64kb

} cpu_t;


/* 

  *******************
  functions to define
  *******************

get_reg8
set_reg8

get_reg16
set_reg16

effective_address

get_operand_value
set_operand_value

execute_mov
execute_add
execute_sub
execute_cmp
execute_jnz

execute_instruction

*/
