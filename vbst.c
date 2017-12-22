#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "vbst.h"
#include "bst.h"

   /*
    *   Francisco Rovelo | Feb, 26
    *   The vbst.c
    */

typedef struct  vbstVal {
	void *value;
	int freq;
	void (*display)(FILE *, void *);
	int (*comp)(void *, void*);
} vbstVal;


vbstVal *newVBSTNode(void (*d)(FILE *,void *),int (*c)(void *,void *)){
	vbstVal *nodeVal = malloc(sizeof(vbstVal));

	nodeVal->value = NULL;
	nodeVal->freq = 1;
	nodeVal->display = d;
	nodeVal->comp = c;
	
	return nodeVal;
}

static void displayVal(FILE *fp, void *val) {
	vbstVal *v = val;
	v->display(fp,v->value);
	if( v->freq > 1) {
		fprintf(fp, "-%d",v->freq);
	}
}

static int vbstCompare(void *a, void *b) {
	vbstVal *A = a;
	vbstVal *B = b;
	
	return A->comp(A->value, B->value);
}
vbst *newVBST(void (*d)(FILE *, void *), int(*c)(void *, void *)) {
	vbst *vtree = malloc(sizeof(vbst));
	vtree->tree = newBST(displayVal, vbstCompare);
	vtree->display = d;
	vtree->compare = c;
	vtree->size = 0;
	vtree->words = 0;

	return vtree;
}
void insertVBST(vbst *vtree,void *v) { //returns inserted node
	vbstVal *urmom = newVBSTNode(vtree->display, vtree->compare);
	urmom->value = v;
	bstNode *temp = findBSTNode(vtree->tree, urmom);

	if(temp == 0) {
		insertBST(vtree->tree,urmom);
		vtree->size++;	
	}
	else ((vbstVal *)(temp->value))->freq++; 

	vtree->words++;
}
int findVBST(vbst *vtree,void *v) {
	vbstVal *urmom = newVBSTNode(vtree->display, vtree->compare);
	urmom->value = v;
	bstNode *temp = findBSTNode(vtree->tree, urmom);
	if(temp == 0) return 0;
	else return ((vbstVal *)(temp->value))->freq; 

}
void deleteVBST(vbst *vtree,void *v){
	vbstVal *urmom = newVBSTNode(vtree->display, vtree->compare);
	urmom->value = v;
	bstNode *temp = findBSTNode(vtree->tree, urmom);
	if(temp == 0) {
		fprintf(stderr, "Value ");
		vtree->display(stderr, urmom->value);
		fprintf(stderr," not found.\n");
	}
	else if(((vbstVal *)(temp->value))->freq > 1) {
		((vbstVal *)(temp->value))->freq--;
		vtree->words--;
	}
	else {
		temp = swapToLeafBSTNode(temp);
		pruneBSTNode(vtree->tree,temp);
		vtree->size--;
		vtree->words--;
	}
}

// int sizeVBST(vbst *vtree)  {
// 	return vtree->size;
// }

int wordsVBST(vbst *vtree) {
	return vtree->words;
}
void statisticsVBST(vbst *vtree,FILE *fp){
	fprintf(fp, "Words/Phrases: %d\n", wordsVBST(vtree));
	//fprintf(fp, "Nodes: %d\n", sizeVBST(vtree));
	statisticsBST(vtree->tree, fp);
}
void displayVBST(FILE *fp,vbst *vtree) {
	displayBST(fp,vtree->tree);
}