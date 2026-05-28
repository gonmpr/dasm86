#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t    s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;


typedef enum {
    REG_A,
    REG_C,
    REG_D,
    REG_B,
    REG_SP,
    REG_BP,
    REG_SI,
    REG_DI,
} reg_t;


typedef enum {
  MEM0_MOD, MEM8_MOD, MEM16_MOD, REG_MOD 
} mod_t;


typedef enum{
  OPERAND_ADDRESS,
  OPERAND_IMMEDIATE,
  OPERAND_REGISTER,
}operand_kind_t;


typedef struct {
  operand_kind_t kind;

  union {
      u16 immediate;
      u16 address;
      reg_t reg;
  };
} operand_t;




typedef struct {

  u8 opcode;
  mod_t mod;
  u8 size;

  operand_t operands[2];

} instruction_t;





