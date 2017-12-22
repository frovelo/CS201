#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "real.h"


real *
newReal(double x)
    {
    real *p = malloc(sizeof(real));
    p->value = x;
    return p;
    }

double getReal(real *v)
    {
    return v->value;
    }

double setReal(real *v,double x)
    {
    double old = v->value;
    v->value = x;
    return old;
    }

void displayReal(FILE *fp,void *v)
    {
    fprintf(fp,"%f",getReal(v));
    }

double compareReal(void *v,void *w)
    {
    return ((real *) v)->value - ((real *) w)->value;
    }

void freeReal(real *v)
    {
    free(v);
    }