#ifndef TOKEN_H
#define TOKEN_H

typedef struct {
  char* type;
  char* literal;
} Token;

Token* LookUpMethod(char* str);
void FreeToken(Token* tok);

#endif
