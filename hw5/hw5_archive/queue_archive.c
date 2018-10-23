#include "queue.h"

struct queue_node
{
	queue_node_t *next;
	queue_node_t *prev;
	void *datum;
};

void pq_init(Queue *pq, int32_t (*compare)(const void *key, const void *with), void (*datum_delete)(void *))
{
	pq->head = NULL;
	pq->size = 0;
	pq->compare = compare;
	pq->datum_delete = datum_delete;

}
void pq_delete(Queue *pq)
{
	if (pq->head)
	{
		queue_node_delete(pq, pq->head);
	}
	pq->head = NULL;
	pq->size = 0;
	pq->compare = NULL;
	pq->datum_delete = NULL;
}
void queue_node_delete(Queue *pq, queue_node_t *node)
{
	queue_node_t *next;
}
void pq_insert(Queue *pq, void *v)
{
}
void *pq_peak(Queue *pq)
{
}
void *pq_pop(Queue *pq)
{
}



/*
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
*/
