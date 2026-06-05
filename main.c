#include <stdio.h>
#include <stdlib.h>
#include "binloader.h"
#include "dasm.h"
#include "text.h"

int main(int argc, char *argv[]){
  if (argc != 2){
  printf("Error: Correct usage: dasm86 <binary_file>\n");
  return 1;
  }



  memory_t binfile = load_file(argv[1]);

  while(binfile.offset < binfile.size){

    instruction_t instruction = decode_instruction(&binfile);

    if(instruction.opcode.kind == OP_INVALID){
      printf("Error: Instruction doesn't exist\n");
      return 1;
    }

    printf("size=%u\n", instruction.size);
    print_instruction(instruction);
    

    binfile.offset += instruction.size;
  }


  free(binfile.data);
  return 0;
}
