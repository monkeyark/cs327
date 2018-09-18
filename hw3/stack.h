#ifndef STACK_H
# define STACK_H

typedef struct stack_node stack_node_t;

struct stack_node {
  int i;
  stack_node_t *next;
};

typedef struct stack {
  int size;
  stack_node_t *top;
} stack_t;

int stack_init(stack_t *s);
int stack_delete(stack_t *s);
int stack_push(stack_t *s, int v);
int stack_pop(stack_t *s, int *v);
int stack_top(stack_t *s, int *v);
int stack_is_empty(stack_t *s);
int stack_size(stack_t *s);

#endif
