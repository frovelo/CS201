#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "integer.h"

/*
*    Francisco Rovelo
*    Dr. Lusth
*    Date: 2017
*
*    This program creates an integer to be used with other .c files in this folder and other 201 folders. 
*
*    This code has been rigorously tested by Dr. John Lusth, University of Alabama Spr. '17
*/

integer *
newInteger(int x)
    {
    integer *p = malloc(sizeof(integer));
    p->value = x;
    return p;
    }

int getInteger(integer *v)
    {
    return v->value;
    }

int setInteger(integer *v,int x)
    {
    int old = v->value;
    v->value = x;
    return old;
    }

void displayInteger(FILE *fp,void *v)
    {
    fprintf(fp,"%d",getInteger((integer *) v));
    }

int compareInteger(void *v,void *w)
    {
    return ((integer *) v)->value - ((integer *) w)->value;
    }

void freeInteger(integer *v)
    {
    free(v);
    }


