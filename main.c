#include <stdio.h>
#include <stdlib.h>
#include "memloader.h"

int main(int argc, char *argv[]){
  if (argc != 2){
  printf("Error: Correct usage: dasm86 <binary_file>");
  }


  char *address_calc[] = {
      "bx + si", "bx + di",
      "bp + si", "bp + di",
      "si", "di", "bp", "bx",
  };

  memory_t assembly = load_file(argv[1]);



  printf("hola, mundo\n");
  free(assembly.data);
  return 0;
}
