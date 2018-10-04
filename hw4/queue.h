#ifndef QUEUE_H
# define QUEUE_H

# include <stdint.h>

struct queue_node;

typedef struct queue_node Node;

typedef struct Priority_Queue
{
  Node *head;
  uint32_t size;
  int32_t (*compare)(const void *key, const void *with);
  void (*datum_delete)(void *);
} Queue;

Node *node_new(int priority);

//Node *node_new_NPC(int priority);

void pq_insert(Queue pq, Node **head, int priority, int *dist);

//void pq_insert_NPC(Queue pq, Node** head , int priority , NPC *npc);

int pq_pop(Queue pq, Node **head);

bool pq_isEmpty(Queue pq, Node **head);

#endif
