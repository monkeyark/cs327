#ifndef QUEUE_H
# define QUEUE_H

typedef struct queue_node queue_node_t;

struct queue_node {
  int i;
  queue_node_t *next;
};

typedef struct queue {
  int size;
  queue_node_t *top;
} queue_t;

int queue_init(queue_t *s);
int queue_delete(queue_t *s);
int queue_push(queue_t *s, int v);
int queue_pop(queue_t *s, int *v);
int queue_top(queue_t *s, int *v);
int queue_is_empty(queue_t *s);
int queue_size(queue_t *s);

#endif
