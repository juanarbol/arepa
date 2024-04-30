#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
  char* input;
  int position;
  int readPosition;
  int lineNumber;
  char ch;
} Lexer;

Lexer* newLexer(char* input);
void freeLexer(Lexer* lexer);

void report_invalid_token(Lexer* lexer);
Token* do_consume_heading(Lexer* lexer);
Token* do_consume_headers(Lexer* lexer);
Token* do_consume_body(Lexer* lexer);

int isDigit(char ch);
int isLetter(char ch);
int isEndline(char ch);
int isSpaceChar(char ch);
void readChar(Lexer* lexer);
void skipSpace(Lexer* lexer);
char peek(Lexer* lexer);
char* readIdentifier(Lexer* lexer);
char* readURL(Lexer* lexer);
char* readVersion(Lexer* lexer);
char* readHeader(Lexer* lexer);
char* readBody(Lexer* lexer);

Token* NextToken(Lexer* lexer);

#endif // LEXER_H
