#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binloader.h"
#include "dasm.h"
#include "emul.h"
#include "text.h"

// gcc -Wall -Wextra -Werror -std=c99 *.c -o dasm86
int main(int argc, char *argv[]){

  if(argc != 2 && argc != 3){
    printf("Error: Correct usage: dasm86 <binary_file> [-run]\n");
    return 1;
  }

  if(argc == 3 && strcmp(argv[2], "-run") != 0){
    printf("Error: Correct usage: dasm86 <binary_file> [-run]\n");
    return 1;
  }


  bool run_program = (argc == 3);

  memory_t binfile = load_file(argv[1]);
  cpu_t cpu = {0};

  if (!run_program) {

    while (binfile.offset < binfile.size) {
      instruction_t ins = decode_instruction(&binfile);
      print_instruction(ins);
      binfile.offset += ins.size;
      }

  }else {

    while (cpu.ip < binfile.size) {
      binfile.offset = cpu.ip;

      instruction_t ins = decode_instruction(&binfile);

      print_instruction(ins);

      u16 old_ip = cpu.ip;

      if (!execute_instruction(&cpu, ins)) {
        printf("Execution error\n");
        return 1;
      }

      if (cpu.ip == old_ip) {
        cpu.ip += ins.size;
      }
    }

  }
  if(run_program){
    print_cpu(&cpu);
  }

  free(binfile.data);
  return 0;
}
