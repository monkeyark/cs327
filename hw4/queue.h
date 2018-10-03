#ifndef QUEUE_H
# define QUEUE_H


struct queue_node;

typedef struct queue_node Node;

typedef struct Priority_Queue
{
  Node *min;
  uint32_t size;
  int32_t (*compare)(const void *key, const void *with);
  void (*datum_delete)(void *);
} Queue;

Node *node_new(Queue pq, int priority);

void pq_insert(Queue pq, Node **head, int priority, int *dist);

int pq_pop(Queue pq, Node **head);

bool pq_isEmpty(Queue pq, Node **head);

#endif
