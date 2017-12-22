#include <stdio.h>
#include <stdlib.h>
#include "darray.h"

struct DArray {
	void **array; //an array of void pointers
	int capacity; //total number of slots
	int size; //number of filled slots
	void (*display)(FILE *,void *);
};

extern DArray *newDArray(void (*display)(FILE *,void *)) {
	DArray *newDA = malloc(sizeof(DArray));
	newDA->array = malloc(sizeof(void *));
	newDA->capacity = 1;
	newDA->size = 0;
	newDA->display = display;

	return newDA;
}
extern void insertDArray(DArray *a,void *v) {
	if(a->size == a->capacity) {
		a->capacity *= 2;
		a->array = realloc(a->array, sizeof(void *) * a->capacity);
		for(int i = a->size; i < a->capacity; i++) {
			a->array[i] = NULL;
		}
	}
	a->array[sizeDArray(a)] = v;
	a->size++;

}
extern void *removeDArray(DArray *a) {
	if(sizeDArray(a) == 0) exit(1);


	void *v = a->array[sizeDArray(a)-1];
	a->size--;
	
	if(a->size*4 < a->capacity && a->capacity != 1) {
		a->capacity /= 2;
		a->array = realloc(a->array, sizeof(void *) * a->capacity);
	}
	return v;
}
extern void *getDArray(DArray *a,int index) {
	if(sizeDArray(a) == 0) return NULL;
	else return a->array[index];
}
extern void setDArray(DArray *a,int index,void *value) {
	if(index == a->size) insertDArray(a,value);
	else {
		a->array[index] = value;
	}
}
extern int sizeDArray(DArray *a) {
	return a->size;
}

extern void displayDArray(FILE *fp,DArray *a) {
	if(a->size == 0) {
		fprintf(fp, "[][%d]", a->capacity);
	}
	else {
		fprintf(fp, "[");
		for(int i = 0; i < a->size; i++) {
			a->display(fp, a->array[i]);
			if(i<a->size-1) fprintf(fp, ",");
		}
		fprintf(fp, "]");
		fprintf(fp, "[");
		fprintf(fp, "%d", a->capacity-a->size);
		fprintf(fp, "]");
	}
}
