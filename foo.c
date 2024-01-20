#include <stdio.h>

typedef void(foo_t)(void);

foo_t foo;

int main(void) {
  foo();
  return 0;
}

void foo_(void) { printf("foo\n"); }
