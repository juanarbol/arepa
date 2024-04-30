#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "parser.h"

typedef struct {
  const char* name;
  void (*test_function)();
} Test;

void run_test(const char* name, void (*test_function)()) {
  printf("Running test: %s\n", name);
  test_function();
  printf("Test %s completed\n\n", name);
}

void assert_true(bool condition, const char* message) {
  if (condition) {
    printf("PASS: %s\n", message);
  } else {
    printf("FAIL: %s\n", message);
  }
}

void assert_false(bool condition, const char* message) {
  assert_true(!condition, message);
}

char* http_request = "GET /foo HTTP/1.1\n"
                     "Host: localhost:4000\n"
                     "User-Agent: curl/7.64.1\n"
                     "Accept: */*\n"
                     "\n"
                     "Hello, World!";

// Define your test functions here
void test_parsing() {
  Parser* parser = newParser(http_request);
  do {
    nextToken(parser);
  } while (parser->peekToken->type != EOF);

  assert_true(strcmp(parser->method, "GET") == 0, "Method should be GET");
  assert_true(strcmp(parser->url, "/foo") == 0, "Url should be /foo");
  assert_true(strcmp(parser->version, "HTTP/1.1") == 0, "Version should be HTTP/1.1");
  assert_true(strcmp(parser->headers[0].key, "Host") == 0, "Host");
  assert_true(strcmp(parser->headers[0].value, "localhost:4000") == 0, "Host should be localhost:8080");
  assert_true(strcmp(parser->headers[1].key, "User-Agent") == 0, "User-Agent");
  assert_true(strcmp(parser->headers[1].value, "curl/7.64.1") == 0, "User-Agent should be curl/7.64.1");
  assert_true(strcmp(parser->headers[2].key, "Accept") == 0, "Accept");
  assert_true(strcmp(parser->headers[2].value, "*/*") == 0, "Accept should be */*");
  assert_true(strcmp(parser->body, "Hello, World!") == 0, "Body should be Hello, World!");
}

int main() {
  Test tests[] = {
    { "Parsing", test_parsing},
  };

  for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
    run_test(tests[i].name, tests[i].test_function);
  }

  return 0;
}
