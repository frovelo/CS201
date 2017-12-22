#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bst.h"
#include "queue.h"
#include "string.h"
/*
*    Francisco Rovelo
*    Dr. Lusth
*    Date: 02/26/2017
*    About:
*	 This program creates a binary search tree.
*
*    	 This code has been rigorously tested by Dr. John Lusth, University of Alabama Spr. '17
*/



//prototype function for statistics 
static int maxDepth(bstNode *n);
static int minDepth(bstNode *n, int count);
// end helper function

bst *newBST(void (*d)(FILE *,void *),int (*c)(void *,void *)) {
	bst *tree = malloc(sizeof(bst));
	tree->root = NULL;
	tree->size = 0;
	tree->compare = c;
	tree->display = d;
	return tree;
}
bstNode *insertBST(bst *tree,void *value) { //returns inserted node
    bstNode *insertN = malloc(sizeof(bstNode));
    bstNode *current = tree->root;
    bstNode *currentP = NULL;
    insertN->value = value;
    insertN->parent = insertN->right = insertN->left = NULL;
    while(current != NULL) {
        currentP = current;
        if(tree->compare(insertN->value,current->value)>=0)  current = current->right;
        else current = current->left;
    }
    tree->size++;
    insertN->parent = currentP;
    if (currentP == NULL) {
        insertN->parent = insertN;
        tree->root = insertN;
    }
    else if(tree->compare(insertN->value, currentP->value) < 0) currentP->left = insertN;
    else currentP->right = insertN;
    insertN->left = NULL;
    insertN->right = NULL;
    return insertN;
}
 int findBST(bst *tree,void *value) { //returns 1 if found, 0 otherwise
		if(tree->root == NULL) return 0;
		int foundN = 0;
		bstNode *n = tree->root;
		while( n!= NULL) {
			if(tree->compare(value,n->value) == 0) {
				foundN = 1;
				break;
			}
			else if(tree->compare(value,n->value) < 0) n = n->left;
			else n = n->right;
		}
		return foundN;
}

bstNode *findBSTNode(bst *tree,void *value) { //returns 0 if not found
		if(tree->root == NULL) return 0;
		int foundN = 0;
		bstNode *n = tree->root;
		while( n!= NULL) {
			if(tree->compare(value,n->value) == 0) {
				foundN = 1;
				break;
			}
			else if(tree->compare(value,n->value) < 0) n = n->left;
			else n = n->right;
		}
		if(foundN == 1) return n;
		else return 0;
}

bstNode *swapToLeafBSTNode(bstNode *n) {//returns leaf node holding the original value
	void *temp;
	bstNode *loc;
	if( n->left == NULL && n->right == NULL) return n;
	else  if(n->left != NULL){
		loc = n->left;
		while( loc->right != NULL) {
			loc = loc->right;
		}
		temp = n->value;
		n->value = loc->value;
		loc->value = temp;

		return swapToLeafBSTNode(loc);
	}
	else {
		loc = n->right;
		while(loc->left != NULL) {
			loc = loc->left;
		}
		temp = n->value;
		n->value = loc->value;
		loc->value = temp;

		return swapToLeafBSTNode(loc);
	}
}

void pruneBSTNode(bst *tree, bstNode *n) { //disconnects n from tree
	tree->size--;

	//n = swapToLeafBSTNode(n);
	if(n == tree->root) {
		tree->root = NULL;
	} 
	else if(n->parent->left == n) {
		 n->parent->left = NULL;
		 n = NULL;
	}
	else {
		n->parent->right = NULL;
		n = NULL;
	}
}

int sizeBST(bst * tree) {
	return tree->size;
}

void statisticsBST(bst *tree,FILE *fp) { //displays min and max depth
	bstNode *current = tree->root;
	int max, min;
	fprintf(fp, "Nodes: %d\n", sizeBST(tree));
	min = minDepth(current, 0);
	fprintf(fp, "Minimum depth: %d\n", min);
	bstNode *current1 = tree->root;
	max = maxDepth(current1);
	fprintf(fp, "Maximum depth: %d\n", max);
}

void displayBST(FILE *fp,bst *tree) { //displays tree, calls display function to display node value
	if(tree->root == NULL) {
		fprintf(fp, "0: \n");
		return;
	}

	int height = 0;
	int tacos = -1;
	bstNode *n = malloc(sizeof(bstNode));
	queue *q = newQueue(tree->display);
	enqueue(q,tree->root);
	enqueue(q,NULL);
	fprintf(fp, "%d:", height);

	while(sizeQueue(q) != 0) {
		n = dequeue(q);
		if(sizeQueue(q) == 0) {
			fprintf(fp, "\n");
			break;
		}
		else if(n == NULL) {
			height++;
			tacos = 0;
			fprintf(fp, "\n");
			enqueue(q,NULL);
		}
		else {
			if(tacos>=0) {fprintf(fp, "%d:", height);}
			fprintf(fp," ");
			if( n->left == NULL && n->right == NULL) fprintf(fp, "=");
			tree->display(fp,n->value);
			fprintf(fp,"("); 
			tree->display(fp,n->parent->value); 
			fprintf(fp,")-");
			if(tree->compare(n->value,n->parent->value)>=0 && height !=0) { fprintf(fp, "r"); }
			else { if(height != 0)fprintf(fp, "l"); }
			if(n->left != NULL) enqueue(q,n->left);
			if(n->right != NULL) enqueue(q,n->right);
			tacos = -1;
		}
	}
}

//helper functions for stats
static int maxDepth(bstNode *n) 
{
   if (n==NULL) return 0;
   else
   {
       /* compute the depth of each subtree */
       int lDepth = maxDepth(n->left);
       int rDepth = maxDepth(n->right);

       /* use the larger one */
       if (lDepth > rDepth) return(lDepth+1);
       else return(rDepth+1);
   }
} 
static int minDepth(bstNode *n, int count)
{
   if (n) {
   	int rDepth = minDepth(n->right,count+1);
   	int lDepth = minDepth(n->left,count +1);

   	if(rDepth > lDepth) return lDepth;
   	else return rDepth;
   }

   return count;
}
//end helper functions
