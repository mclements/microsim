#include <assert.h>
#include <stdio.h>
#include "pqueue.h"

struct item {
	int value;
	double priority;
};

static int initialized = 0;
static int front, rear;

struct item items[PQUEUE_CAPACITY]; /*increasing sequence (according to priority)*/

void pqueue_init(void)
{
	front = 0;
	rear = 0;
	initialized = 1;
}


int pqueue_count(void)
{
	int result;
	
	assert(initialized);
	
	if (front <= rear) {
		result = rear - front;
	} else {
		result = rear + PQUEUE_CAPACITY - front;
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


static int increasing_items()
{
	int result, i;
	
	if (pqueue_count() < 2) {
		result = 1;
	} else {
		i = front;
		while ((i != previous(rear)) && (items[i].priority <= items[next(i)].priority)) {
			i++;
		}
		result = i == previous(rear);
	}
	return result;
}


void pqueue_insert(int value, double priority)
{
	int i, j, old_count = pqueue_count();
	
	assert(initialized);
	assert(old_count < PQUEUE_CAPACITY);
	
	/*insert the new item last in the queue*/
	items[rear].value = value;
	items[rear].priority = priority;
	
	if (old_count > 0) {
		/*swap items until the new item is not smaller than the one before it*/
		i = previous(rear);
		j = rear;
		while ((j != front) && (priority < items[i].priority)) {
			swap(&(items[i]), &(items[j]));
			j = i;
			i = previous(i);
		}
	}
	rear = next(rear);

	assert(pqueue_count() == old_count + 1);
	assert(increasing_items());
}


void pqueue_remove(int *value, double *priority)
{
	int old_count = pqueue_count();

	assert(initialized);
	assert(old_count > 0);
	
	*value = items[front].value;
	*priority = items[front].priority;
	front = next(front);

	assert(pqueue_count() == old_count - 1);
	assert(increasing_items());
}


void pqueue_print(void)
{
	int i;
	
	printf("(");
	for (i = front; i != rear; i = next(i)) {
		if (i != front) {
			printf(", ");
		}
		printf("(%f, %d)", items[i].priority, items[i].value);
	}
	printf(")");
}
