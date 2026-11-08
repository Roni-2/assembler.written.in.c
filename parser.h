#pragma once

#include <stdbool.h>

#define MAX_BINARY_SIZE 256

typedef unsigned char u8;

extern u8 final_binary[MAX_BINARY_SIZE];

bool parse_lines();

int get_binary_size();

void print_binary();

void write_binary(char *file_name);

