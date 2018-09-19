/*
#ifndef QUEUE_H
# define QUEUE_H

typedef struct queue_node Node;

struct queue_node
{
	int data;
	int priority;
	Node *next;
};

typedef struct queue
{
	int size;
	Node *peek;
} priority_queue;

int queue_init(priority_queue *s);
int queue_delete(priority_queue *s);
int queue_push(priority_queue *s, int v);
int queue_pop(priority_queue *s, int *v);
int queue_peek(priority_queue *s, int *v);
int queue_isEmpty(priority_queue *s);
int queue_size(priority_queue *s);

#endif
*/
