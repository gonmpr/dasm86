#pragma once
#include <stdint.h>
#include <stdbool.h>

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
    REG_A,
    REG_B,
    REG_C,
    REG_D,
    REG_SP,
    REG_BP,
    REG_SI,
    REG_DI,
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
  bool w;

  union {
      u16 immediate;
      u16 address;
      reg_t reg;
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
  u8 size;

  mod_t mod;
  operand_t dst;
  operand_t src;

} instruction_t;



opcode_t get_opcode(unsigned char byte);

instruction_t decode_instruction(memory_t *mem);
