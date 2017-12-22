#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bst.h"

   /*
    *   Francisco Rovelo | Feb, 26
    *   The rbt.h for rbt.c
    */

    #ifndef __RBT_INCLUDED__
    #define __RBT_INCLUDED__
    typedef struct rbt
        {
        bst *tree;
        void (*display)(FILE *,void *);
        int (*compare)(void *,void *);
        int size;
        int words;
        } rbt;

    extern rbt *newRBT(void (*)(FILE *,void *),int (*)(void *,void *));
    extern void insertRBT(rbt *,void *);
    extern int findRBT(rbt *,void *);
    extern void deleteRBT(rbt *,void *);
    extern void statisticsRBT(rbt *,FILE *);
    extern void displayRBT(FILE *,rbt *);
   // extern void checkRBT(rbt *);               //optional
    #endif