/*A minimum priority queue data structure*/

#define PQUEUE_CAPACITY 10 /*maximum number of items that can be stored in the queue*/

void pqueue_init(void); /*initializes and empties the queue*/

void pqueue_insert(int item, double priority); /*inserts an item into the queue*/

void pqueue_remove(int *item, double *priority); /*removes the item with the highest priority and returns it and its priority*/

int pqueue_count(void); /*returns the number of items currently in the queue*/

void pqueue_print(void); /*prints the queue to stdout*/
