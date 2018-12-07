#include <stdio.h>

#include "stack.h"

int main(int argc, char *argv[])
{
  stack_t s;
  int i, j;

  stack_init(&s);
  for (i = 0; i < 10; i++) {
    stack_push(&s, i);
  }

  printf("Size is %d\n", stack_size(&s));

  for (i = 0; i < 5; i++) {
    stack_pop(&s, &j);
    printf("Popped %d\n", j);
  }
  printf("Size is %d\n", stack_size(&s));

  stack_delete(&s);

  return 0;
}
