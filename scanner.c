#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"

Token tokens[MAX_TOKENS];

void read_file(char *filename, char **buffer) {
  long length;
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    printf("Could not open file %s\n", filename);
    exit(1);
  }
  fseek(f, 0, SEEK_END);
  length = ftell(f);
  fseek(f, 0, SEEK_SET);
  *buffer = malloc(length + 1);
  if (*buffer == NULL) {
    exit(2);
  }
  fread(*buffer, 1, length, f);
  (*buffer)[length] = '\0'; 
  fclose(f);
}

struct Scanner {
  char *start;
  char *current;
  int line;
} scanner;

void init_scanner(char *file_name) {
  char *file_content;
  read_file(file_name, &file_content);
  scanner.start = file_content;
  scanner.current = file_content;
  scanner.line = 1;
}

char peek_char() {
  return *scanner.current;
}

char consume_char() {
  char c = *scanner.current;
  scanner.current += 1;
  return c;
}

int get_scanner_length() {
  return (int)(scanner.current - scanner.start);
}

Token create_token(TokenType type, int value) {
  Token token;
  token.type = type;
  token.value = value;
  token.source = scanner.start;
  token.line = scanner.line;
  token.length = get_scanner_length();
  return token;
}

void consume_comment() {
  while (peek_char() != '\n' && peek_char() != '\0') {
    consume_char();
  }
}

void consume_whitespace() {
  while (true) {
    char c = peek_char();
    if (c == ';') {
      consume_comment();
      continue;
    }
    if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
      return;
    if (c == '\n')
      scanner.line += 1;
    consume_char();
  }
}

bool is_digit(char c) {
  return c >= '0' && c <= '9';
}

bool is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool is_alpha_numeric(char c) {
  return is_alpha(c) || is_digit(c);
}

Token create_int(char c) {
  int result = c - '0';
  while (is_digit(peek_char())) {
    c = consume_char();
    if (result < 100000)
      result = (result * 10) + (c - '0'); 
  }
  return create_token(TOKEN_INTEGER, result);
}

bool is_token_equal_to(char* s) {
  int scan_len = get_scanner_length();
  if (scan_len != strlen(s))
    return false;
  for (int i=0; i<scan_len; i++)
    if (s[i] != scanner.start[i])
      return false;
  return true;
}

Token create_identifier() {
  while (is_alpha_numeric(peek_char())) {
    consume_char();
  }

  if (is_token_equal_to("mov"))
    return create_token(TOKEN_MOV, 0);
  if (is_token_equal_to("not"))
    return create_token(TOKEN_NOT, 0);
  if (is_token_equal_to("const"))
    return create_token(TOKEN_CONST, 0);
  if (is_token_equal_to("or"))
    return create_token(TOKEN_ALU, 0);
  if (is_token_equal_to("add"))
    return create_token(TOKEN_ALU, 1);
  if (is_token_equal_to("sub"))
    return create_token(TOKEN_ALU, 2);
  if (is_token_equal_to("nand"))
    return create_token(TOKEN_ALU, 3);
  if (is_token_equal_to("and"))
    return create_token(TOKEN_ALU, 4);
  if (is_token_equal_to("xor"))
    return create_token(TOKEN_ALU, 6);
  if (is_token_equal_to("eq"))
    return create_token(TOKEN_ALU, 7);
  if (is_token_equal_to("reg1"))
    return create_token(TOKEN_REGISTER, 1);
  if (is_token_equal_to("reg2"))
    return create_token(TOKEN_REGISTER, 2);
  if (is_token_equal_to("reg3"))
    return create_token(TOKEN_REGISTER, 3);
  if (is_token_equal_to("reg4"))
    return create_token(TOKEN_REGISTER, 4);
  if (is_token_equal_to("reg5"))
    return create_token(TOKEN_REGISTER, 5);
  if (is_token_equal_to("reg6"))
    return create_token(TOKEN_REGISTER, 6);

  return create_token(TOKEN_IDENTIFIER, 0);
}

Token scan_token() {
  consume_whitespace();

  scanner.start = scanner.current;
  char c = consume_char();

  if (c == ',')
    return create_token(TOKEN_COMMA, 0);
  if (c == '=')
    return create_token(TOKEN_EQUALS, 0);
  if (c == '[')
    return create_token(TOKEN_LEFT_BRACKET, 0);
  if (c == ']')
    return create_token(TOKEN_RIGHT_BRACKET, 0);
  if (is_digit(c))
    return create_int(c);
  if (is_alpha(c))
    return create_identifier();
  if (c == '\0')
    return create_token(TOKEN_EOF, 0);
  return create_token(TOKEN_UNKNOWN, 0);
}

void scan_tokens() {
  for (int i=0; true; i++) {
    if (i == MAX_TOKENS) {
      printf("Compile error: file has too many tokens, the limit is %d.\n", MAX_TOKENS);
      exit(1);
    }
    tokens[i] = scan_token();
    if (tokens[i].type == TOKEN_EOF)
      return;
  }
}
