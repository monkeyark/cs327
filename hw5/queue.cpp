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

void pq_delete(Queue pq)
{
	while (!pq_isEmpty(pq, pq.head))
	{
		pq_pop(pq, pq.head);
	}

	pq.head = NULL;
}

/*
Node_t *node_new_NPC(int priority, Character c)
{
	Node_t *temp = malloc(sizeof(Node_t));
	temp->priority = priority;
	temp->character = c;
	temp->next = NULL;

	return temp;
}

void pq_insert_NPC(Queue_npc pq, Node_t** head, Node_t** new)
{
	Node_t *temp = *head;

	int head_priority = (*head)->priority;
	int new_priority = (*new)->priority;
	if (head_priority == new_priority)
	{
		head_priority = (*head)->character.birth;
		new_priority = (*new)->character.birth;
	}

	if (head_priority > new_priority)
	{
		(*new)->next = *head;
		*head = *new;
		*pq.head = *head;
	}
	else
	{
		int c = temp->next->priority;
		int d = (*new)->priority;
		while (temp->next != NULL && (c < d))
		{
			temp = temp->next;
		}

		(*new)->next = temp->next;
		temp->next = (*new);
	}
}

Node_t *pq_pop_NPC(Queue_npc pq, Node_t **head)
{
	int priority = (*head)->priority;
	Node_t *t = *head;
	(*head) = (*head)->next;
	(*pq.head) = (*head);
	free(t);

	int next_turn = priority + 1000 / (*head)->character.speed;
	Character h = (*head)->character;

	return node_new_NPC(next_turn, h);
}

bool pq_isEmpty_NPC(Queue_npc pq, Node_t *head)
{
	return head == NULL;
}

void pq_delete_NPC(Queue_npc pq)
{

	while (!pq_isEmpty_NPC(pq, pq.head))
	{
		pq_pop_NPC(pq, pq.head);
	}
}
*/
