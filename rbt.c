#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "integer.h"
#include "rbt.h"

   /*
    *   Author: Francisco Rovelo
    *	Date: Feb, 26
    *   About:
    *		Creates a Red Black Tree based off of a Binary Search Tree. Takes a void* value. Look at BST.c for more thoro-
    *		ugh comments or look up how a RBT operates(I hope this code is self explanatory, probably not)
    */


//static int getColor(bstNode*);
static int getParentColor(bstNode*);
static int getUncleColor(bstNode*);
static void setParentColor(bstNode*, int);
static void setGrandParentColor(bstNode*, int);
static void setUncleColor(bstNode*, int);
static void setColor(bstNode*, int);
static int isLeftChild(bstNode*);
//static int isRightChild(bstNode*);
static bstNode* getParent(bstNode*);
static bstNode* getGrandParent(bstNode*);
static bstNode* getUncle(bstNode*);
static bstNode* rightChild(bstNode*);
static bstNode* leftChild(bstNode*);
//static void rightRotate(bstNode*);
//static void leftRotate(bstNode*);
static void rotate(bst *, bstNode *, bstNode *);
//static void rotateToParent(bstNode*);


typedef struct  rbtVal {
	void *value;
	int freq;
	int color;
	void (*display)(FILE *, void *);
	int (*compare)(void *, void*);
} rbtVal;

rbtVal *newRBTNode(void (*d)(FILE *,void *),int (*c)(void *,void *)){
	rbtVal *nodeVal = malloc(sizeof(rbtVal));

	nodeVal->value = NULL;
	nodeVal->freq = 1;
	nodeVal->color = 1; // 0 is black 1 is red
	nodeVal->display = d;
	nodeVal->compare = c;
	
	return nodeVal;
}

static void insertionFixup(bst *bstree, bstNode* x) {
	while(1) {
        // x is the root
        if (bstree->root == x) break;
        // parent is black
        else if (getParentColor(x) == 0) {
            break;
        }
        // uncle is red recoloring case
        if (getUncleColor(x) == 1)
        {
            setParentColor(x, 0);
            setUncleColor(x, 0);
            setGrandParentColor(x, 1);
            x = getGrandParent(x);
        }
        // else uncle is black
        else
        {
            bstNode* parent = getParent(x);
            bstNode* gparent = getGrandParent(x);
            bstNode* oparent = getParent(x);
            bstNode* oldx = x;
            if((rightChild(parent) == x) && (leftChild(gparent)) == parent) {
                rotate(bstree, x,parent);
                x = oparent;
                parent = oldx;
            }
            else if((leftChild(parent) == x) && (rightChild(gparent) == parent)) {
                rotate(bstree, x, parent);
                x = oparent;
                parent = oldx;
            }

            setColor(getParent(x), 0);
            setColor(getGrandParent(x), 1);

            if(getParent(x) == leftChild(getGrandParent(x))) {
                rotate(bstree, parent, getParent(parent));
            }
            else {
                rotate(bstree, parent, getParent(parent));
            }

            break;
        }
    }
    // root is always black
    setColor(bstree->root, 0);
}

static void displayVal(FILE* fp, void* val) {
	rbtVal *v = val;
	v->display(fp,v->value);
	if( v->freq > 1) {
		fprintf(fp, "-%d",v->freq);
	}
	if(v->color == 0) fprintf(fp, "-B");
	else fprintf(fp, "-R");
}

static int rbtCompare(void *a, void *b) {
	rbtVal *A = a;
	rbtVal *B = b;
	
	return A->compare(A->value, B->value);
}

static int sizeRBT(rbt *rbtree)  {
	return rbtree->size;
}

static int wordsRBT(rbt *rbtree) {
	return rbtree->words;
}

rbt *newRBT(void (*d)(FILE *,void *),int (*c)(void *,void *)) {
	rbt *rbtree = malloc(sizeof(rbtVal));

	rbtree->tree = newBST(displayVal, rbtCompare);
	rbtree->display = d;
	rbtree->compare = c;
	rbtree->size = 0;
	rbtree->words = 0;

	return rbtree;
}

void insertRBT(rbt *rbtree,void *v) {
	rbtVal *urmom = newRBTNode(rbtree->display, rbtree->compare);
	urmom->value=v;
	bstNode *temp = findBSTNode(rbtree->tree, urmom);
	if(temp == 0) {
		insertionFixup(rbtree->tree,insertBST(rbtree->tree, urmom));
		rbtree->size++;

	}
	else ((rbtVal *)(temp->value))->freq++;
	rbtree->words++;

}

int findRBT(rbt *rbtree,void *v) {
	rbtVal *urmom = newRBTNode(rbtree->display, rbtree->compare);
	urmom->value = v;
	bstNode *temp = findBSTNode(rbtree->tree, urmom);
    if( temp ==0) return 0;
	else return ((rbtVal *)(temp->value))->freq; 
}

void deleteRBT(rbt *rbtree,void *v) {
    rbtVal *urmom = newRBTNode(rbtree->display, rbtree->compare);
    urmom->value = v;
}

void statisticsRBT(rbt *rbtree,FILE *fp) {
	fprintf(fp, "Words/Phrases: %d\n", wordsRBT(rbtree));
	statisticsBST(rbtree->tree, fp);
}

void displayRBT(FILE *fp,rbt *rbtree) {
	displayBST(fp,rbtree->tree);
}

static void rotate(bst *bstree, bstNode *a, bstNode *b) {
    if (b->left == a) { // right potatoe
        b->left = a->right;
        if(b->left) b->left->parent = b;
        if(b->parent != b) a->parent = b->parent;
        else {
            a->parent = a;
            bstree->root = a;
        }
        if(a->parent->left == b) a->parent->left = a;
        else a->parent->right = a;
        a->right = b;
        b->parent = a;
    }
    else { //lefti loosy
        b->right = a->left;
        if(b->right) b->right->parent = b;
        if(b->parent != b) a->parent = b->parent;
        else {
            a->parent = a;
            bstree->root = a;
        }
        if(a->parent->right == b) a->parent->right = a;
        else a->parent->left = a;
        a->left = b;
        b->parent = a;
    }
}

// void checkRBT(rbt *tree);       //optional

/*static void rightRotate(bstNode* a) {
    bstNode* aRight = a->right;   // is the NULL?
    bstNode* b = a->parent;

    if (b->parent)
    {
        a->parent = b->parent;

        if (isRightChild(b)) a->parent->right = a;
        else a->parent->left = a;
        if (aRight)
        {
            b->left = aRight;
            aRight->parent = b;
        }
        else
        {
            b->left = NULL;
        }
        b->parent = a;
        a->right = b;
    }
    else        // b is root
    {
        bstNode* newB = (bstNode*)malloc(sizeof(bstNode));
        newB->value = b->value;
        newB->right = b->right;

        if (b->right)
            b->right->parent = newB;
            
        newB->left = a->right;
        
        if (newB->left)
            newB->left->parent = newB;
        
        newB->parent = b;
        b->right = newB;
        b->value = a->value;
        b->left = a->left;
        b->left->parent = b;
    }
}

static void leftRotate(bstNode* a) {
    printf("Inside left rotate: \n");
    bstNode* aLeft = a->left;   // is the NULL?
    bstNode* b = a->parent;

    if (b->parent)
    {
        printf("Inside left rotate if: \n");
        a->parent = b->parent;

        if (isLeftChild(b)) a->parent->left = a;
        else a->parent->right = a;
        if (aLeft)
        {
            b->right = aLeft;
            aLeft->parent = b;
        }
        else
        {
            b->right = NULL;
        }
        b->parent = a;
        a->left = b;
    }
    else        // b is root
    {
        printf("Inside left rotate else: \n");
        bstNode* newB = (bstNode*)malloc(sizeof(bstNode));
        newB->value = b->value;
        newB->left = b->left;

        if (b->left)
            b->left->parent = newB;
            
        newB->right = a->left;
        
        if (newB->right)
            newB->right->parent = newB;
        
        newB->parent = b;
        b->left = newB;
        b->value = a->value;
        b->right = a->right;
        b->right->parent = b;
    }
}
*/
static int getParentColor(bstNode* node) {
    bstNode* p = node->parent;
    if (p)
    {
        rbtVal* val = p->value;
        if (val)
            return val->color;
        else return 0;
    }
    return 0;
}

static bstNode* getParent(bstNode* node) {
    if(node->parent == node) return 0;
    else if (node->parent) return node->parent;
    else return 0;
}

static bstNode* getGrandParent(bstNode* node) {
    if (node->parent->parent) return node->parent->parent;
    else return 0;
}

static bstNode* getUncle(bstNode* node) {
    if (isLeftChild(getParent(node)))
    {
        bstNode* rc = rightChild(getGrandParent(node));
        if (rc)
        {
            return rightChild(getGrandParent(node));
        }
        else
        {
            return 0;
        }
    }
    else
    {   
        bstNode* lc = leftChild(getGrandParent(node));
        if (lc)
        {
            return leftChild(getGrandParent(node));
        }
        else 
        {
            return 0;
        }
    }
}

static int getUncleColor(bstNode* node) {
    bstNode* u = getUncle(node);
    if (u)
    {
        rbtVal* val = u->value;
        if (val)
            return val->color;
        else return 0;
    }
    return 0;
}

static void setParentColor(bstNode* node, int c) {
    ((rbtVal *)(node->parent->value))->color = c;
}

static void setUncleColor(bstNode* node, int c) {
    bstNode* u = getUncle(node);
    rbtVal* val = u->value;
    val->color = c;
}

static void setGrandParentColor(bstNode* node, int c) {
    bstNode* gp = getGrandParent(node);
    ((rbtVal *)(gp->value))->color = c;
}

static bstNode* rightChild(bstNode* node) {
    if (node->right) return node->right;
    else return 0;
}

static bstNode* leftChild(bstNode* node) {
    if (node->left) return node->left;
    else return 0;
}

static int isLeftChild(bstNode* node) {
    bstNode* p = getParent(node);
    if (p->left == node) return 1;
    else return 0;
}

// static int isRightChild(bstNode* node) {
//     bstNode* p = getParent(node);
//     if (p->right == node) return 1;
//     else return 0;
// }

// static void rotateToParent(bstNode* node) {
//     if (isLeftChild(node))
//         rightRotate(node);   // change back
//     else
//         leftRotate(node);
// }

static void setColor(bstNode* node, int c) {
    ((rbtVal *)(node->value))->color = c;

}

// static int getColor(bstNode* node) {
//     rbtVal* val = node->value;
//     if (val) return val->color;
//     else return 0;
// }
