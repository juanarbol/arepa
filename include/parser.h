#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include "lexer.h"
#include "token.h"

typedef struct {
  char* key;
  char* value;
} Header;

typedef struct {
  Lexer* lexer;
  Token* currToken;
  Token* peekToken;
  char* method;
  char* url;
  char* version;
  char* body;
  Header* headers;
  size_t num_headers;
} Parser;

void nextToken(Parser* parser);
void freeParser(Parser* parser);
void append_header(Parser* parser, Token* token);
Parser* newParser(char* input);

#endif
