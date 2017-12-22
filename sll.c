#include <stdlib.h>
#include <stdio.h>
#include "sll.h"
#include "integer.h"

/*
*    Francisco Rovelo
*    Dr. Lusth
*    Date: 2017
*    About:
*       This program creates a singly Linked List with a head and tail pointer. It holds a generic value, and it can ins-
*       ert and remove at index. Through union with a donor and a recipient two list can be joined. Please look at dll.c for
*       more thorough comments.
*
*       This code has been rigorously tested by Dr. John Lusth, University of Alabama Spr. '17
*/

sll *newSLL(void (*d)(FILE *,void *)) //d is the display function
{
    sll *items = malloc(sizeof(sll));
    if (items == 0)
    {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    items->head = 0;
    items->tail = 0;
    items->size = 0;
    items->display = d;
    return items;
}
void insertSLL(sll *items,int index,void *value) //stores a generic value
{   
    sllnode *temp = malloc(sizeof(sllnode));
    
    // insert into an empty list
    if( index == 0 && items->head == NULL){
        items->head = temp;
        items->tail = temp;
        temp->value = value;
        temp->next = NULL;
    }
    //insert at head
    else if (index == 0 && items->head != NULL) {
        temp->next = items->head;
        temp->value = value;
        items->head=temp;
    }
    //insert at tail
    else if( index == sizeSLL(items)) {
        items->tail->next = temp;
        temp->value = value;
        items->tail = temp;
        items->tail->next=NULL;
    }
    //insert inbetween head&tail
    else {
        sllnode *post, *prev;
        prev = items->head;
        for( int i=0; i<index-1;i++)
        {
            prev=prev->next;
        }
        post = prev->next;
        temp->next = post;
        prev->next=temp;
        temp->value = value;
    }
    items->size++;
}
void *removeSLL(sll *items,int index)            //returns a generic value
{
    sllnode *post= items->head;
    sllnode *temp= items->head;

    if(index==0 && items->size != 0){
        items->head= items->head->next;
        items->size= items->size -1;
        return temp->value;
    }
    else if(index == items->size - 1){
        for(int i=0;i < sizeSLL(items)-2;i++){
            post = post ->next;
        }
        temp=items->tail;
        post->next=NULL;
        items->tail=post;
        items->size= items->size - 1;
        return temp->value;
    }
    else{
        for(int i=0;i<index - 1;i++){
            post=post->next;
        }   
        temp = post->next;
        post->next=post->next->next;
        items->size= items->size - 1;
        return temp->value;
    }
}
void unionSLL(sll *recipient,sll *donor)         //merge two lists into one
{
    if(donor->size == 0)
        return;
    else if(recipient->size == 0) {
        recipient->head = donor->head;
        recipient->tail = donor->tail;
    }
    else {
        recipient->tail->next = donor->head;
        recipient->tail = donor->tail;
    }
     recipient->size += donor->size;
     donor->head = NULL;
     donor->tail = NULL;
     donor->size = 0;
}
void *getSLL(sll *items,int index)               //get the value at the index
{
    sllnode *taco;
    taco = items->head;
    for( int i = 0; i < index-1; i++) {
        taco = taco->next;
    }
    return taco->value;
}
int sizeSLL(sll *items)
{
    return items->size;
}
void displaySLL(FILE *fp,sll *items)
{
    sllnode * temp;
    temp = items->head;
    
    fprintf(fp,"[");
    while( temp != NULL) {
        items -> display(fp, temp->value);
        if(temp->next != NULL) {
            fprintf(fp,",");
        }
        temp = temp->next;
    }
    fprintf(fp,"]");
}
