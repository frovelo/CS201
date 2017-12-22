#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "string.h"

/*
*    Francisco Rovelo
*    Dr. Lusth
*    Date: 02/2017
*
*
*
*    To all ye who enter, 
*
*    This file angered me.
*
*    newString() allocates memory for a pointer p and allocates memory the size of
*    strcpy(x) then sets p->value to x.
*
*    The other's are pretty self explanatory after that information- particuarly setString... which I didn't even use.
*
*    This code has been rigorously tested by Dr. John Lusth, University of Alabama Spr. '17
*/


strings* newString(char *x) {
	strings *p = malloc(sizeof(strings));
	p->value = malloc(sizeof(strlen(x)));
	strcpy(p->value,x);
	return p;
}

char* getString(strings *v){
	return v->value;
}

char* setString(strings *v, char *x){
    char* old = malloc(sizeof(strlen(v->value)));
    strcpy(old,v->value);
    free(v->value);
    v->value = malloc(sizeof(strlen(x)));
    strcpy(v->value,x);
    return old;
}

void displayString(FILE * fp,void * v){
    fprintf(fp,"\"%s\"",getString((strings *) v));
}
