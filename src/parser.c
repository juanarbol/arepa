#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"

void nextToken (Parser* parser) {
  parser->currToken = parser->peekToken;
  parser->peekToken = NextToken(parser->lexer);
}

void freeParser(Parser *parser) {
  freeLexer(parser->lexer);
  free(parser);
}

Parser* newParser(char* input) {
  Parser* parser = malloc(sizeof(Parser));
  if (parser == NULL) return NULL;

  parser->lexer = malloc(sizeof(Lexer));
  if (parser->lexer == NULL) {
    free(parser);
    return NULL;
  }

  return parser;
}
