#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"


Node *node_new(int priority)
{
	Node *temp = malloc(sizeof(Node));
	temp->priority = priority;
	temp->next = NULL;

	return temp;
}

void pq_insert(Queue pq, Node **head, int priority, int *dist)
{
	Node *temp = *head;
	Node *new = node_new(priority);

	if (dist[(*head)->priority] >= dist[new->priority])
	{
		new->next = (*head);
		(*head) = new;
	}
	else
	{
		while (temp->next != NULL && dist[temp->next->priority] < dist[new->priority])
		{
			temp = temp->next;
		}

		new->next = temp->next;
		temp->next = new;
	}
}

int pq_pop(Queue pq, Node **head)
{
	int n = (*head)->priority;
	Node *t = *head;
	(*head) = (*head)->next;
	free(t);
	return n;
}

bool pq_isEmpty(Queue pq, Node **head)
{
	return (*head) == NULL;
}
