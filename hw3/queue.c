/*
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

int queue_init(priority_queue *s)
{
	s->size = 0;
	s->peek = NULL;

	return 0;
}

int queue_delete(priority_queue *s)
{
	Node *n;

	while (s->peek)
	{
		n = s->peek;
		s->peek = s->peek->next;
		free(n);
	}
	s->size = 0;

	return 0;
}

int queue_push(priority_queue *s, int v, int p)
{
	Node *n;

	if (!(n = malloc(sizeof(*n))))
	{
		return 1;
	}

	s->size++;
	n->next = s->peek;
	s->peek = n;
	n->data = v;

	return 0;
}

int queue_pop(priority_queue *s, int *v)
{
	Node *n;

	if (!s->peek)
	{
		return 1;
	}

	*v = s->peek->data;
	s->size--;
	n = s->peek;
	s->peek = s->peek->next;
	free(n);

	return 0;
}

int queue_peek(priority_queue *s, int *v)
{
	if (!s->peek)
	{
		return 1;
	}

	*v = s->peek->data;

	return 0;
}

int queue_isEmpty(priority_queue *s)
{
	return !s->peek;
}

int queue_size(priority_queue *s)
{
	return s->size;
}
*/
