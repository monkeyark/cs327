#ifndef QUEUE_H
# define QUEUE_H

# include <stdint.h>

#include "dungeon.h"

struct queue_node;

typedef struct queue_node Node;

struct queue_node
{
	Node *next;
	int priority;
};

typedef struct Priority_Queue
{
  Node *head;
  uint32_t size;
  int32_t (*compare)(const void *key, const void *with);
  void (*datum_delete)(void *);
} Queue;

Node *node_new(int priority);

void pq_insert(Queue pq, Node **head, int priority, int *dist);

int pq_pop(Queue pq, Node **head);

bool pq_isEmpty(Queue pq, Node **head);


typedef struct Node_t
{
	struct Node_t *next;
	int priority;
	Character character;
} Node_t;


Node_t *node_new_NPC(int priority, Character c);

void pq_insert_NPC(Queue pq, Node_t** head, Node_t** new);

Node_t *pq_pop_NPC(Queue pq, Node_t **head);

#endif
