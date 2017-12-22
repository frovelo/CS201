#include <stdio.h>
#include <stdlib.h>
#include "binomial.h"
#include "darray.h"
#include "queue.h"

/*
 *
 *
 *
 *
 *
*/


//PROTOTYPE
static void findExtreme(Binomial *b);
static void updateExtreme(Binomial *b, BinomialNode *n);
static void consolidate(Binomial *b, BinomialNode *n);
static BinomialNode *combine(Binomial *b, BinomialNode *x, BinomialNode *y );
static void merge(Binomial *b, DArray *donor);
static BinomialNode *bubbleUp(Binomial *b, BinomialNode *n);
static void *getSubHeap(DArray *donor, int index);
static void printLevelOrder(FILE *fp, BinomialNode *n);
static int getNodeDegree(BinomialNode *n);
//END

struct BinomialNode{
    void *value;
    DArray *children;
    struct BinomialNode *parent;
    void (*display)(FILE *,void *);
};

struct Binomial {
    DArray *rootlist;
    int (*compare)(void *,void *);
    void (*update)(void *,BinomialNode *);
    void (*display)(FILE *,void *);
    BinomialNode *extreme;
    int size;
};

BinomialNode *newBinomialNode(void (*display)(FILE *,void *),void *value) {
    BinomialNode *n = malloc(sizeof(BinomialNode));
    if(n == 0)  {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    n->value = value;
    n->children = newDArray(NULL);
    n->display = display;
    n->parent = n;

    return n;
}

Binomial *newBinomial(void (*dVertex)(FILE *,void *),int (*cVertex)(void *,void *), void (*uVertex)(void *,BinomialNode *)) {
    Binomial *b = malloc(sizeof(Binomial));
    if(b == 0)  {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    b->display = dVertex;
    b->compare = cVertex;
    b->update = uVertex; // update vertex
    b->rootlist = newDArray(NULL);
    b->extreme = NULL;
    b->size = 0;

    return b;
}

void displayBinomialNode(FILE *fp,BinomialNode *n) {

    n->display(fp, n->value);
    fprintf(fp, "-%d", getNodeDegree(n));
    if (n->parent != n) {
        fprintf(fp, "(");
        n->parent->display(fp, n->parent->value);
        fprintf(fp, "-%d)", getNodeDegree(n->parent));
     }
} 

BinomialNode *insertBinomial(Binomial *b,void *value) {
    BinomialNode *temp = newBinomialNode(b->display, value);
    consolidate(b, temp);
    b->size++;
    updateExtreme(b, temp);

    return temp;
}

int sizeBinomial(Binomial *b) {
    return b->size;
}

void deleteBinomial(Binomial *b,BinomialNode *n) {
    decreaseKeyBinomial(b,n, NULL);
    extractBinomial(b);
}

void decreaseKeyBinomial(Binomial *b,BinomialNode *n,void *value) {
    n->value = value;
    n = bubbleUp(b,n);
    findExtreme(b);
}

void *extractBinomial(Binomial *b) {
    BinomialNode *y = b->extreme;
    void *temp;
    //setDArray(b->rootlist,getNodeDegree(y),NULL);
    for(int i = 0; i < sizeDArray(b->rootlist); i++) {
        if(y == getSubHeap(b->rootlist, i)) {
            setDArray(b->rootlist,i,NULL);
            break;
        }
    }
    merge(b, y->children);
    b->size--;
    temp = y->value;
    free(y);
    findExtreme(b);

    return temp;
}

void displayBinomial(FILE *fp,Binomial *b) {
    BinomialNode *n;
    if (sizeBinomial(b) == 0) {
    fprintf(fp,"0:\n");
        return;
    }
    for(int i = 0; i < sizeDArray(b->rootlist); i++) {
        n = getSubHeap(b->rootlist,i);
        if( n != NULL) {
            printLevelOrder(fp,n);
            fprintf(fp, "----\n");
        }
    }
}

//
//
// PRIVATE FUNCTIONS
//
//

int getNodeDegree(BinomialNode *n) {
    return sizeDArray(n->children);
}
void printLevelOrder(FILE *fp, BinomialNode *n) {
    queue *q = newQueue(n->display);
    BinomialNode *temp;

    enqueue(q, n);
    enqueue(q, NULL);
    int level = 0;
    fprintf(fp, "%d: ", level);

    while (sizeQueue(q) > 0) {
        temp = dequeue(q);
        if(sizeQueue(q) == 0) {
            fprintf(fp,"\n");
            break;
        }
        else if (temp == NULL) {
            fprintf(fp,"\n");
            enqueue(q,NULL);
            ++level;
            if (sizeQueue(q) > 1) {
                fprintf(fp,"%d: ",level);
            }
        }
        else {
            displayBinomialNode(fp, temp);
            if(peekQueue(q) != NULL) fprintf(fp, " ");
            for(int i = 0; i < sizeDArray(temp->children); i++) {
                enqueue(q,(BinomialNode*)getSubHeap(temp->children,i));
            }
        }
    }
}

void *getSubHeap(DArray *donor, int index) {
    if(index > sizeDArray(donor)) exit(1);
    if(index == sizeDArray(donor)) return NULL;
    return getDArray(donor, index);

}

void findExtreme(Binomial *b) {
    b->extreme = NULL;
    for(int i = 0;i < sizeDArray(b->rootlist); i++) {
        updateExtreme(b,getSubHeap(b->rootlist, i));
    }
}

void updateExtreme(Binomial *b, BinomialNode *n) {
    if(n == NULL) return;
    if(b->extreme == NULL) b->extreme = n;
    else if(b->compare(n->value, b->extreme->value) < 0) b->extreme = n;
}

void consolidate(Binomial *b, BinomialNode *n) {
    int d = sizeDArray(n->children);

    while(d < sizeDArray(b->rootlist) && getSubHeap(b->rootlist, d) != NULL) {
        n = combine(b, n, getSubHeap(b->rootlist, d));
        setDArray(b->rootlist, d, NULL);
        d++;
    }
    setDArray(b->rootlist, d, n);
    findExtreme(b);
}

BinomialNode *combine(Binomial *b, BinomialNode *x, BinomialNode *y ) {
    if(b->compare(x->value,y->value) < 0) {
        int d = sizeDArray(y->children);
        setDArray(x->children, d, y);
        y->parent = x;
        return x;
    }
    else {
        int d = sizeDArray(x->children);
        setDArray(y->children, d, x);
        x->parent = y;
        return y;
    }
}

void merge(Binomial *b, DArray *donor) {
    BinomialNode *temp = newBinomialNode(b->display, NULL);
    for(int i = 0; i < sizeDArray(donor); i++) {
        //((BinomialNode*)getSubHeap(donor, i))->parent = ((BinomialNode*)getSubHeap(donor, i));
        temp = getSubHeap(donor, i);
        temp->parent = temp;
        //consolidate(b, getSubHeap(donor, i));
        consolidate(b, temp);
    }
    free(donor);
}

BinomialNode *bubbleUp(Binomial *b, BinomialNode *n) {
    // for(int i = 0; i < sizeDArray(b->rootlist); i++) {
    //  if(n == getDArray(b->rootlist, i)) return n;
    // }
    if(n->parent == n || b->compare(n->value,n->parent->value) > 0) return n;
    else {
        b->update(n->value,n->parent);
        b->update(n->parent->value, n);
        //BinomialNode *temp = malloc(sizeof(BinomialNode));
        void *val;
        // temp->value = n->parent->value;
        // n->parent->value = n->value;
        // n->value = temp->value
        val = n->parent->value;
        n->parent->value = n->value;
        n->value = val;

        return bubbleUp(b, n->parent);
    }
}

//
//
// END
//
//