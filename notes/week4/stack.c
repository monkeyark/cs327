#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

/* WARNING!!!  THERE ARE BUGS IN THIS CODE!!!  WE WILL DEBUG IT IN LECTURE *
 * NEXT WEEK, AND A BUG-FREE IMPLEMENTATION WILL BE INCLUDED IN THE WEEK 5 *
 * NOTES!!!                                                                */

int stack_init(stack_t *s)
{
  s->size = 0;
  s->top = NULL;

  return 0;
}

int stack_delete(stack_t *s)
{
  stack_node_t *n;

  while (s->top) {
    n = s->top;
    s->top = s->top->next;
    free(n);
  }
  s->size = 0;

  return 0;
}

int stack_push(stack_t *s, int v)
{
  stack_node_t *n;

  if (!(n = malloc(sizeof (*n)))) {
    return 1;
  }

  s->size++;
  n->next = s->top;
  s->top = n;
  n->i = v;

  return 0;
}

int stack_pop(stack_t *s, int *v)
{
  stack_node_t *n;
 
  if (!s->top) {
    return 1;
  }

  *v = s->top->i;
  s->size--;
  n = s->top;
  s->top = s->top->next;
  free(n);

  return 0;
}

int stack_top(stack_t *s, int *v)
{
  if (!s->top) {
    return 1;
  }

  *v = s->top->i;

  return 0;  
}

int stack_is_empty(stack_t *s)
{
  return !s->top;
}

int stack_size(stack_t *s)
{
  return s->size;
}
