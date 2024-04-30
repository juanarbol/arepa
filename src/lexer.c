#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

Lexer* newLexer(char* input) {
  // Init and set the input
  Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
  lexer->input = input;
  lexer->readPosition = 0;
  lexer->lineNumber = 0;

  // Fill the next token and current token
  readChar(lexer);

  return lexer;
}

void freeLexer(Lexer* lexer) {
  free(lexer);
}

// NOTE: I know this is like phisying with a granade, strtok would make my file
// way easier, but for now, I will just use it for the header parsing.
Token* NextToken(Lexer* lexer) {
  // Before cutting the \n, make sure we don't mess up the body parsing
  int is_body = peek(lexer) == '\n' && lexer->ch == '\n';
  Token* tok = (Token*)malloc(sizeof(Token));
  if (tok == NULL) return NULL;

  skipSpace(lexer);

  if (is_body) {
    tok = do_consume_body(lexer);
    if (tok == NULL) return NULL;
    return tok;
  }

  if (lexer->ch == '\0') {
    tok->type = "EOF";
    tok->literal = "";
    return tok;
  }

  if (lexer->lineNumber == 0) {
    tok = do_consume_heading(lexer);
  } else {
    tok = do_consume_headers(lexer);
  }

  if (tok == NULL) return NULL;
  if (strcmp(tok->type, "INVALID") == 0)
    report_invalid_token(lexer);

  return tok;
}

Token* do_consume_body(Lexer* lexer) {
  Token* tok = (Token*)malloc(sizeof(Token));
  if (tok == NULL) return NULL;

  char* body = readBody(lexer);
  if (body == NULL) return NULL;

  tok->type = "BODY";
  tok->literal = body;

  return tok;
}

Token* do_consume_headers(Lexer* lexer) {
  Token* tok = (Token*)malloc(sizeof(Token));
  if (tok == NULL) return NULL;

  char* header = readHeader(lexer);
  if (header == NULL) return NULL;

  tok->type = "HEADER";
  tok->literal = header;

  return tok;
}

Token* do_consume_heading(Lexer* lexer) {
  Token* tok = (Token*)malloc(sizeof(Token));
  if (tok == NULL) return NULL;

  // The method
  if (lexer->position == 0) {
    char* method = readIdentifier(lexer);
    if (method == NULL) return NULL;

    tok = LookUpMethod(method);
    return tok;
  }

  // Either URL either HTTP version
  if (lexer->ch == '/') {
    char* ident = readURL(lexer);
    if (ident == NULL) return NULL;

    tok->literal = (char*)malloc(strlen(ident) + 1);
    if (tok->literal == NULL) {
      // Handle memory allocation failure
      free(tok);
      return NULL;
    }
    tok->type = "URL";
    strcpy(tok->literal, ident);
    return tok;
  }

  // Parse the HTTP/X.Y
  if (lexer->ch == 'H') {
    char* version = readVersion(lexer);
    if (version == NULL) return NULL;
    tok->type = "VERSION";
    tok->literal = version;
    return tok;
  }

  return tok;
}

void skipSpace(Lexer* lexer) {
  while (lexer->ch == ' ' ||
         lexer->ch == '\t' ||
         lexer->ch == '\n' ||
         lexer->ch == '\r') {
    if (lexer->ch == '\n') lexer->lineNumber++;
    readChar(lexer);
  }
}

char* readVersion(Lexer* lexer) {
  int pos = lexer->position;
  char* identifier = malloc(sizeof(char));
  // Read HTTP/X.Y
  while (isLetter(lexer->ch) ||
         isDigit(lexer->ch) ||
         lexer->ch  == '.' ||
         lexer->ch  == '/') {
    readChar(lexer);
  }

  strncpy(identifier, lexer->input + pos, lexer->position - pos);
  return identifier;
}

char* readURL(Lexer* lexer) {
  int pos = lexer->position;
  char* identifier = malloc(sizeof(char));
  // Skip the / first
  do {
    readChar(lexer);
  } while (isLetter(lexer->ch));

  strncpy(identifier, lexer->input + pos, lexer->position - pos);
  return identifier;
}

char* readBody(Lexer* lexer) {
  int pos = lexer->position;
  char* identifier = malloc(sizeof(char));
  while (lexer->ch != '\n' && lexer->ch != '\0') {
    readChar(lexer);
  }

  strncpy(identifier, lexer->input + pos, lexer->position - pos);
  return identifier;
}

char* readHeader(Lexer* lexer) {
  int pos = lexer->position;
  char* identifier = malloc(sizeof(char));
  while (lexer->ch != '\n' && lexer->ch != '\0') {
    readChar(lexer);
  }

  strncpy(identifier, lexer->input + pos, lexer->position - pos);
  return identifier;
}

char* readIdentifier(Lexer* lexer) {
  int pos = lexer->position;
  char* identifier = malloc(sizeof(char));
  while (isLetter(lexer->ch)) readChar(lexer);

  strncpy(identifier, lexer->input + pos, lexer->position - pos);
  return identifier;
}

char peek (Lexer* lexer) {
  if (lexer->readPosition >= strlen(lexer->input)) return 0;

  return lexer->input[lexer->readPosition];
}

void readChar(Lexer* lexer) {
  if (lexer->readPosition >= strlen(lexer->input)) {
    lexer->ch = '\0';
  } else {
    lexer->ch = lexer->input[lexer->readPosition];
  }

  // Move to the next character
  lexer->position = lexer->readPosition;
  lexer->readPosition = lexer->readPosition + 1;
}

void report_invalid_token(Lexer* lexer) {
  // Print the line containing the error for context
  int startOfLine = lexer->readPosition;
  while (startOfLine > 0 && lexer->input[startOfLine - 1] != '\n') {
    startOfLine--;
  }

  int endOfLine = lexer->readPosition;
  while (lexer->input[endOfLine] != '\0' && lexer->input[endOfLine] != '\n') {
    endOfLine++;
  }

  fprintf(stderr,
          "Line %d: %.*s\n",
          lexer->lineNumber,
          endOfLine - startOfLine,
          &lexer->input[startOfLine]);

  // Print the pointer (^) to indicate the error position
  int errorPosition = lexer->readPosition - startOfLine;
  fprintf(stderr, "%*s^ Here\n", errorPosition, "");

  freeLexer(lexer);
  exit(EXIT_FAILURE);
}

int isEndline (char ch) {
  return ch == '\n';
}

int isLetter (char ch) {
  // Allow -
  return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || (ch == '-');
}

int isDigit (char ch) {
  return '0' <= ch && ch <= '9';
}
