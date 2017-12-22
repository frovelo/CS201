#include <stdlib.h>
#include <stdio.h>
#include "sll.h"
#include "queue.h"
#include "integer.h"

/*
*    Francisco Rovelo
*    Dr. Lusth
*    Date: 2017
*    About:
*       This program creates a queue using a singly linked list. It holds a generic value.
*
*       This code has been rigorously tested by Dr. John Lusth, University of Alabama Spr. '17
*/



struct queue *newQueue(void (*d)(FILE *,void *))   //constructor
{
    queue *items = malloc(sizeof(sll));
    if (items == 0)
    {
        fprintf(stderr, "out of memory");
        exit(-1);
    }
    items->list = newSLL(d);
    return items;
    
}
void enqueue(queue *items,void *value)      //stores a generic value
{
    insertSLL(items->list, items->list->size, value);
}
void *dequeue(queue *items)                 //returns a generic value
{
    return removeSLL(items->list,0);
}
void *peekQueue(queue *items)               //returns a generic value
{
    return getSLL(items->list,0);
}
int sizeQueue(queue *items)
{
    return sizeSLL(items->list);
}
void displayQueue(FILE *fp,queue *items)
{
    displaySLL(fp, items->list);
}
