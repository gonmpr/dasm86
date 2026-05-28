#pragma once
#include <stddef.h>
#include <stdio.h>

typedef struct {
  unsigned char *data;
  size_t size;
  size_t offset;
} memory_t;


size_t get_file_size(FILE *fp);
memory_t load_to_memory(FILE *fp, size_t file_size);
memory_t load_file(char *filename);
