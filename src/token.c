#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "token.h"

const char* METHOD_LIST[8] = {
  "OPTIONS",
  "GET",
  "HEAD",
  "POST",
  "PUT",
  "DELETE",
  "TRACE",
  "CONNECT"
};

void FreeToken (Token* token) {
  free(token);
}

Token* LookUpMethod (char* str) {
  int i;
  Token* tok = (Token*)malloc(sizeof(Token));
  tok->type = "INVALID";
  tok->literal = "";

  for (i = 0; i < sizeof(METHOD_LIST) / sizeof(METHOD_LIST[0]); i++) {
    if (strcmp(METHOD_LIST[i], str) == 0) {
      tok->type = "METHOD";
      tok->literal = strdup(METHOD_LIST[i]);
      return tok;
    }
  }

  return tok;
}
