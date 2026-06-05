#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "binloader.h"

//just renaming int types
typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t    s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;


//registers
typedef enum {

    REG_AL,
    REG_CL,
    REG_DL,
    REG_BL,

    REG_AH,
    REG_CH,
    REG_DH,
    REG_BH,

    REG_AX,
    REG_CX,
    REG_DX,
    REG_BX,

    REG_SP,
    REG_BP,
    REG_SI,
    REG_DI,

    REG_INVALID,
} reg_t;


// modes for the instruction
typedef enum {
  MEM0_MOD, MEM8_MOD, MEM16_MOD, REG_MOD 
} mod_t;

// for the instruction operands
typedef enum{
  OPERAND_ADDRESS,
  OPERAND_IMMEDIATE,
  OPERAND_REGISTER,
  OPERAND_INVALID,
}operand_kind_t;

// for instruction decoding in opcode_t struct
typedef enum{
  OP_INVALID,
  OP_INM_RM,
  OP_MOV,
  OP_ADD,
  OP_SUB,
  OP_CMP,
  OP_JNZ,
}instruction_kind_t;







//for the effective address calculation
typedef struct {

    reg_t base;
    reg_t index;

    u16 displacement;

} memory_operand_t;


// for the operands of the instruction
typedef struct {
  operand_kind_t kind;

  union {
      u16 immediate;
      reg_t reg;
      memory_operand_t mem;
  };
} operand_t;


// the opcode for decoding
typedef struct {

    u8 mask;
    u8 pattern;

    instruction_kind_t kind;

} opcode_t;



//the instruction representation, used for printing the text
typedef struct {

  opcode_t opcode;


  operand_t operands[2]; //first is src, second is dst

  u8 size;

} instruction_t;



instruction_t decode_instruction(memory_t *mem);
