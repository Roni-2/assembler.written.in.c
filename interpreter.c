#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"

u8 registers[8];
u8 ram[256];

void check_register(int regis, int address) {
  if (regis < 1 || regis > 6) {
    printf("Runtime error at byte %d: there is no register %d.\n", address, regis);
    exit(1);
  }
}

u8 calculate(int operation, u8 a, u8 b) {
  switch (operation) {
    case 0: return a | b;
    case 1: return a + b;
    case 2: return a - b;
    case 3: return ~(a & b);
    case 4: return a & b;
    case 5: return ~a;
    case 6: return a ^ b;
    default: return a == b;
  }
}

void print_state() {
  for (int i=1; i<=6; i++) {
    printf("reg%d = %d\n", i, registers[i]);
  }

  for (int i=0; i<256; i++) {
    if (ram[i] != 0) {
      printf("ram[%d] = %d\n", i, ram[i]);
    }
  }
}

void run_bytecode(u8 *code, int length) {
  for (int i=0; i<length; i++) {
    u8 instruction = code[i];
    int kind = instruction >> 6;
    int a = (instruction >> 3) & 0b111;
    int b = instruction & 0b111;

    if (kind == 0) {
      registers[1] = instruction & 0b111111;
    } else if (kind == 1) {
      check_register(a, i);
      registers[a] = calculate(b, registers[1], registers[2]);
    } else if (kind == 2) {
      check_register(a, i);
      check_register(b, i);
      registers[b] = registers[a];
    } else if (instruction == 0b11000000) {
      registers[1] = ram[registers[2]];
    } else if (instruction == 0b11000001) {
      ram[registers[2]] = registers[1];
    } else {
      printf("Runtime error at byte %d: unknown instruction %d.\n", i, instruction);
      exit(1);
    }
  }

  print_state();
}

void run_bytecode_file(char *file_name) {
  u8 code[MAX_BINARY_SIZE];

  FILE *f = fopen(file_name, "rb");
  if (f == NULL) {
    printf("Could not open file %s\n", file_name);
    exit(1);
  }
  int length = fread(code, 1, MAX_BINARY_SIZE, f);

  if (fgetc(f) != EOF) {
    printf("%s is too big, the limit is %d bytes.\n", file_name, MAX_BINARY_SIZE);
    exit(1);
  }
  fclose(f);

  run_bytecode(code, length);
}
