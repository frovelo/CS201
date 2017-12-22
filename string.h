#include <stdio.h>

#ifndef __STRING_INCLUDED__
#define __STRING_INCLUDED__

typedef struct strings {
    char* value;
} strings;

extern strings *newString(char*);
extern char* getString(strings *);
extern char* setString(strings *, char*);
extern void displayString(FILE *,void *);

#endif