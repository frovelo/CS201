#include <stdio.h>
#include <stdlib.h>
#include "comparator.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include <string.h>


/*
*    Francisco Rovelo
*    Dr. Lusth
*    Date: 2017
*
*    Comparator.c is just that. It's functions to compare ints, strings, or reals(doubles) that are passed through.
*    Everything here is pretty self explanatory. -1,1,0 are returned to work with sqsort in sqsort.c
*    
*    The only trickery may be in stringComparator- val1 and val2 are casted from strings to char * and uses strcmp. 
*    If the result is positive it returns -1, if it's negative returns 1 and if the same returns 0
*
*    This code has been rigorously tested by Dr. John Lusth, University of Alabama Spr. '17
*/

int intComparator(void *v1, void *v2) {
    int val1 = getInteger(v1);
    int val2 = getInteger(v2);
    
    if(val1 > val2) return -1;
    else if(val1 < val2) return 1;
    else return 0;
}

int stringComparator(void *s1, void *s2) {
    char * val1 = ((strings *) s1)->value;
    char * val2 = ((strings *) s2)->value;
    int result;
    result = strcmp(val1, val2);
    if(result > 0) return -1;
    else if(result < 0) return 1;
    else return 0;
}

int realComparator(void *v1, void *v2) {
    double val1 = getReal(v1);
    double val2 = getReal(v2);
    
    if(val1 > val2) return -1;
    else if(val1 < val2) return 1;
    else return 0;
}

