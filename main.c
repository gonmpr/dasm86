#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "binloader.h"
#include "dasm.h"
#include "emul.h"
#include "text.h"

 
int main(int argc, char *argv[])
{
  bool run_program = false;
  bool dump_memory = false;
  char *filename = NULL;

  for(int i = 1; i < argc; i++) {
    if(strcmp(argv[i], "-r") == 0) {
      run_program = true;
    }else if(strcmp(argv[i], "-rw") == 0) {
      run_program = true;
      dump_memory = true;
    }else {
      filename = argv[i];
    }
  }

  if(filename == NULL) {
    printf("Error: Correct usage: dasm86 [-run] [-write] <binary_file>\n");
    return 1;
  }

  memory_t binfile = load_file(filename);
  if(binfile.data == NULL) {
    return 1;
  }

  cpu_t cpu = {0};

  if(!run_program) {
    while(binfile.offset < binfile.size) {
      instruction_t ins = decode_instruction(&binfile);

      if(ins.opcode.kind == OP_INVALID || ins.size == 0) {
          printf("Error: invalid instruction at offset %zu byte=%02X\n",
                 binfile.offset,
                 binfile.data[binfile.offset]);
          free(binfile.data);
          return 1;
      }

        print_instruction(ins);
        binfile.offset += ins.size;
    }
  } else {
    while(cpu.ip < binfile.size) {
      binfile.offset = cpu.ip;

      instruction_t ins = decode_instruction(&binfile);

      if(ins.opcode.kind == OP_INVALID || ins.size == 0) {
        printf("Error: invalid instruction at offset %u byte=%02X\n",
               cpu.ip,
               binfile.data[cpu.ip]);
        free(binfile.data);
        return 1;
      }

      print_instruction(ins);

      u16 old_ip = cpu.ip;

      if(!execute_instruction(&cpu, ins)) {
          printf("Execution error\n");
          free(binfile.data);
          return 1;
      }

      if(cpu.ip == old_ip) {
          cpu.ip += ins.size;
      }
    }

    print_cpu(&cpu);
  }

  if(dump_memory) {
    bool all_ok = write_memfile(cpu);
    if(!all_ok){
      free(binfile.data);
      return 1;
    }
  }

  free(binfile.data);
  return 0;
}
