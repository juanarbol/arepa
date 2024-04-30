#ifndef PARSER_H
#define PARSER_H

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
  Header headers[];
} Parser;

void nextToken(Parser* parser);
void freeParser(Parser* parser);
Parser* newParser(char* input);

#endif
