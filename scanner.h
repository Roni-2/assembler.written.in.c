#pragma once

#define MAX_TOKENS 10000

typedef enum {
  TOKEN_REGISTER,
  TOKEN_INTEGER,
  TOKEN_IDENTIFIER,
  TOKEN_MOV,
  TOKEN_NOT,
  TOKEN_ALU, 
  TOKEN_CONST,
  TOKEN_COMMA,
  TOKEN_EQUALS,
  TOKEN_LEFT_BRACKET,
  TOKEN_RIGHT_BRACKET,
  TOKEN_UNKNOWN,
  TOKEN_EOF,
} TokenType;

typedef struct {
  TokenType type;
  int value;
  char *source;
  int length;
  int line;
} Token;

extern Token tokens[MAX_TOKENS];

void init_scanner(char *file_name);

void scan_tokens();

