#ifndef QUEUE_H
# define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


struct queue_node;
typedef struct queue_node queue_node_t;

typedef struct priority_queue
{
  queue_node_t *head;
  uint32_t size;
  int32_t (*compare)(const void *key, const void *with);
  void (*datum_delete)(void *);
} Queue;

void pq_init(Queue *pq, int32_t (*compare)(const void *key, const void *with), void (*datum_delete)(void *));
void pq_delete(Queue *pq);
void pq_insert(Queue *pq, void *v);
void *pq_peak(Queue *pq);
void *pq_pop(Queue *pq);




/*
struct queue_node_npc;

typedef struct queue_node_npc Node_t;

struct queue_node_npc
{
	Node_t *next;
	int priority;
	Character character;
};

typedef struct Priority_Queue_npc
{
  Node_t **head;
  uint32_t size;
  int32_t (*compare)(const void *key, const void *with);
  void (*datum_delete)(void *);
} Queue_npc;



Node_t *node_new_NPC(int priority, Character c);

void pq_insert_NPC(Queue_npc pq, Node_t** head, Node_t** new);

Node_t *pq_pop_NPC(Queue_npc pq, Node_t **head);

void pq_delete_NPC(Queue_npc pq);
*/
#endif
