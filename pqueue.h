/*A minimum priority queue data structure*/

#define PQUEUE_CAPACITY 10 /*maximum number of items that can be stored in a queue*/

struct pqueue;

struct pqueue *pqueue_new(void); /*returns a new empty queue*/

void pqueue_clear(struct pqueue *q); /*empties the queue*/

void pqueue_insert(int item, double priority, struct pqueue *q); /*inserts an item into the queue*/

void pqueue_remove(struct pqueue *q, int *item, double *priority); /*removes the item with the highest priority and returns it and its priority*/

int pqueue_count(struct pqueue *q); /*returns the number of items currently in the queue*/

void pqueue_print(struct pqueue *q); /*prints the queue to stdout*/

void pqueue_delete(struct pqueue *q); /*frees the resources occupied by the queue*/
