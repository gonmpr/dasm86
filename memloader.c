#include "memloader.h"
#include <stdlib.h>
#include <stdio.h>


size_t get_file_size(FILE *fp){

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp); 
  rewind(fp);

  return (size_t)size;
}


memory_t load_to_memory(FILE *fp, size_t file_size) {
  memory_t loaded_file = {0};

  if (!fp) {
    return loaded_file; //          couldn't open the file
  }

  unsigned char *buffer = malloc(file_size);
  if (!buffer) {
    return loaded_file; //          couldn't locate memory
  }


  size_t bytes_read = fread(buffer, 1, file_size, fp);
  
  loaded_file.data = buffer; 
  loaded_file.size = bytes_read;
  loaded_file.offset = 0;

  return loaded_file;
}

memory_t load_file(char *filename){
  memory_t mem_file = {0};
  FILE *fp = fopen(filename, "rb");
  if (!fp){
    return mem_file;
  } 

  size_t file_size = get_file_size(fp);
  mem_file = load_to_memory(fp, file_size);
  fclose(fp);
  return mem_file;
}

