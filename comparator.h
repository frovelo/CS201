#include <stdio.h>

#ifndef __COMPARATOR_INCLUDED__
#define __COMPARATOR_INCLUDED__

typedef int (*Comparator)(void*,void*);

int intComparator(void *x, void *y);
int stringComparator(void *x, void *y);
int realComparator(void *x, void*y);


#endif
