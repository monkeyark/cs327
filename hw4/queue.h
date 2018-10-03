#ifndef QUEUE_H
# define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node
{
	struct Node *next;
	int priority;
} Node;

Node *node_new(int priority);

void pq_insert(Node **head, int priority, int *dist);

int pq_pop(Node **head);

bool pq_isEmpty(Node **head);

#endif
