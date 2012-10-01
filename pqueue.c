#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "pqueue.h"

struct item {
	int value;
	double priority;
};

struct pqueue {
	int front, rear;
	struct item items[PQUEUE_CAPACITY]; /*increasing sequence (according to priority)*/
};

struct pqueue *pqueue_new(void)
{
	struct pqueue *q;
	
	q = malloc(sizeof *q);
	if (q != NULL) {
		q->front = 0;
		q->rear = 0;
	}
	return q;
}


void pqueue_clear(struct pqueue *q)
{
	q->front = 0;
	q->rear = 0;
}


int pqueue_count(struct pqueue *q)
{
	int result;
	
	if (q->front <= q->rear) {
		result = q->rear - q->front;
	} else {
		result = q->rear + PQUEUE_CAPACITY - q->front;
	}
	assert((result >= 0) && (result <= PQUEUE_CAPACITY));
	return result;
}


static int previous(int i)
{
	return (i - 1) % PQUEUE_CAPACITY;
}


static int next(int i)
{
	return (i + 1) % PQUEUE_CAPACITY;
}


static void swap(struct item *first, struct item *second)
{
	int value;
	double priority;
	
	value = first->value;
	first->value = second->value;
	second->value = value;
	
	priority = first->priority;
	first->priority = second->priority;
	second->priority = priority;
}


static int increasing_items(struct pqueue *q)
{
	int result, i;
	
	if (pqueue_count(q) < 2) {
		result = 1;
	} else {
		i = q->front;
		while ((i != previous(q->rear)) && (q->items[i].priority <= q->items[next(i)].priority)) {
			i++;
		}
		result = i == previous(q->rear);
	}
	return result;
}


void pqueue_insert(int value, double priority, struct pqueue *q)
{
	int i, j, old_count = pqueue_count(q);
	
	assert(old_count < PQUEUE_CAPACITY);
	
	/*insert the new item last in the queue*/
	q->items[q->rear].value = value;
	q->items[q->rear].priority = priority;
	
	if (old_count > 0) {
		/*swap items until the new item is not smaller than the one before it*/
		i = previous(q->rear);
		j = q->rear;
		while ((j != q->front) && (priority < q->items[i].priority)) {
			swap(&(q->items[i]), &(q->items[j]));
			j = i;
			i = previous(i);
		}
	}
	q->rear = next(q->rear);

	assert(pqueue_count(q) == old_count + 1);
	assert(increasing_items(q));
}


void pqueue_remove(struct pqueue *q, int *value, double *priority)
{
	int old_count = pqueue_count(q);

	assert(old_count > 0);
	
	*value = q->items[q->front].value;
	*priority = q->items[q->front].priority;
	q->front = next(q->front);

	assert(pqueue_count(q) == old_count - 1);
	assert(increasing_items(q));
}


void pqueue_print(struct pqueue *q)
{
	int i;
	
	printf("(");
	for (i = q->front; i != q->rear; i = next(i)) {
		if (i != q->front) {
			printf(", ");
		}
		printf("(%f, %d)", q->items[i].priority, q->items[i].value);
	}
	printf(")");
}


void pqueue_delete(struct pqueue *q)
{
	free(q);
}
