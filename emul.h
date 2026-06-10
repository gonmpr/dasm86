#pragma once

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


bool execute_instruction(cpu_t *cpu, instruction_t ins);
bool write_memfile(cpu_t cpu);
