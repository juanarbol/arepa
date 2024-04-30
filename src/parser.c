#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"
#include "token.h"

void append_header(Parser* parser, Token* token) {
  parser->headers = realloc(parser->headers, (parser->num_headers + 1) * sizeof(Header));
  if (parser->headers == NULL) return;

  // Split the header into key and value
  char* header_copy = strdup(token->literal);
  if (header_copy == NULL) {
    return;
  }

  // Assuming the header format is "key: value"
  char* colon_pos = strchr(header_copy, ':');
  if (colon_pos == NULL) {
    free(header_copy);
    return;
  }

  *colon_pos = '\0';

  // Trim leading and trailing whitespace from key and value
  char* key = header_copy;
  while (*key && *key == ' ') key++;
  char* end_key = header_copy + strlen(key) - 1;
  while (end_key > key && *end_key == ' ') end_key--;
  *(end_key + 1) = '\0';

  char* value = colon_pos + 1;
  while (*value && *value == ' ') value++;

  // Trim trailing spaces at the end of the value
  char* end_value = header_copy + strlen(token->literal) - 1;
  while (end_value >= value && *end_value == ' ') end_value--;
  *(end_value + 1) = '\0';

  parser->headers[parser->num_headers].key = strdup(key);
  parser->headers[parser->num_headers].value = strdup(value);
  parser->num_headers++;

  free(header_copy);
}

void nextToken (Parser* parser) {
  parser->currToken = parser->peekToken;
  parser->peekToken = NextToken(parser->lexer);

  if (parser->currToken == NULL && parser->peekToken == NULL) return;

  switch (parser->peekToken->type) {
    case METHOD:
      parser->method = parser->peekToken->literal;
      break;
    case URL:
      parser->url= parser->peekToken->literal;
      break;
    case VERSION:
      parser->version = parser->peekToken->literal;
      break;
    case BODY:
      parser->body = parser->peekToken->literal;
      break;
    case HEADER:
      append_header(parser, parser->peekToken);
      break;
    default:
      break;
  }
}

void freeParser(Parser *parser) {
  freeLexer(parser->lexer);
  free(parser);
}

Parser* newParser(char* input) {
  Parser* parser = malloc(sizeof(Parser));
  if (parser == NULL) return NULL;

  parser->lexer = newLexer(input);
  if (parser->lexer == NULL) {
    free(parser);
    return NULL;
  }

  parser->headers = NULL;
  parser->num_headers = 0;

  return parser;
}
