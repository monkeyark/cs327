#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

/* WARNING!!!  THERE ARE BUGS IN THIS CODE!!!  WE WILL DEBUG IT IN LECTURE *
 * NEXT WEEK, AND A BUG-FREE IMPLEMENTATION WILL BE INCLUDED IN THE WEEK 5 *
 * NOTES!!!                                                                */

int queue_init(queue_t *s)
{
  s->size = 0;
  s->top = NULL;

  return 0;
}

int queue_delete(queue_t *s)
{
  queue_node_t *n;

  while (s->top) {
    n = s->top;
    s->top = s->top->next;
    free(n);
  }
  s->size = 0;

  return 0;
}

int queue_push(queue_t *s, int v)
{
  queue_node_t *n;

  if (!(n = malloc(sizeof (*n)))) {
    return 1;
  }

  s->size++;
  n->next = s->top;
  s->top = n;
  n->i = v;

  return 0;
}

int queue_pop(queue_t *s, int *v)
{
  queue_node_t *n;
 
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

int queue_top(queue_t *s, int *v)
{
  if (!s->top) {
    return 1;
  }

  *v = s->top->i;

  return 0;  
}

int queue_is_empty(queue_t *s)
{
  return !s->top;
}

int queue_size(queue_t *s)
{
  return s->size;
}
