#ifndef TOKEN_H
#define TOKEN_H

enum Type {
  METHOD,
  URL,
  VERSION,
  HEADER,
  BODY,
  INVALID
};

typedef struct {
  enum Type type;
  char* literal;
} Token;

Token* LookUpMethod(char* str);
void FreeToken(Token* tok);

#endif
