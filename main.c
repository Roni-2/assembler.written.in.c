#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"
#include "parser.h"
#include "interpreter.h"

void print_usage() {
  printf("Usage: main [flags]\n");
  printf("  -f <file>  assembly file to compile (default: example.asm)\n");
  printf("  -o <file>  also write the bytecode to a file\n");
  printf("  -r         run the program after compiling it\n");
  printf("  -i <file>  don't compile, just run a bytecode file\n");
  printf("  -h         show this message\n");
}

char *get_flag_value(int argc, char **argv, int i) {
  if (i + 1 >= argc) {
    printf("Flag %s needs a file name after it\n", argv[i]);
    exit(1);
  }
  return argv[i + 1];
}

int main(int argc, char **argv) {
  char *input_file = "example.asm";
  char *output_file = NULL;
  char *bytecode_file = NULL;
  bool run = false;

  for (int i=1; i<argc; i++) {
    if (strcmp(argv[i], "-f") == 0) {
      input_file = get_flag_value(argc, argv, i);
      i++;
    } else if (strcmp(argv[i], "-o") == 0) {
      output_file = get_flag_value(argc, argv, i);
      i++;
    } else if (strcmp(argv[i], "-i") == 0) {
      bytecode_file = get_flag_value(argc, argv, i);
      i++;
    } else if (strcmp(argv[i], "-r") == 0) {
      run = true;
    } else if (strcmp(argv[i], "-h") == 0) {
      print_usage();
      return 0;
    } else {
      printf("Unknown flag %s\n", argv[i]);
      print_usage();
      return 1;
    }
  }

  if (bytecode_file != NULL) {
    run_bytecode_file(bytecode_file);
    return 0;
  }

  init_scanner(input_file);
  scan_tokens();
  if (!parse_lines()) {
    return 1;
  }
  print_binary();

  if (output_file != NULL) {
    write_binary(output_file);
  }

  if (run) {
    run_bytecode(final_binary, get_binary_size());
  }

  return 0;
}

