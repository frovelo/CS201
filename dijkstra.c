#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdarg.h>
#include <limits.h>
#include <ctype.h>
#include "integer.h" 
#include "darray.h"
#include "binomial.h"
#include "comparator.h"
#include "scanner.h"
#include "queue.h"
#include "sll.h"



typedef struct vertexNode {
	DArray *neighborList;
	int vertex;
	int visited;
	int steps;
	int distance; //key
	struct vertexNode *prev;
	struct BinomialNode *position;
} vertexNode;

typedef struct neighborNode {
	int vertexN;
	int eWeight;
	int visited;
} neighborNode;


static vertexNode *newVertexNode(int value) {
	vertexNode *n = malloc(sizeof(vertexNode));
	n->vertex = value;
	n->neighborList = newDArray(displayInteger);
	n->position = NULL;
	n->prev = NULL;
	n->visited = 0;
	n->steps = 0;
	n->distance = INT_MAX;

	return n;
}
static neighborNode *newNeighborNode(int value, int weight) {
	neighborNode *n = malloc(sizeof(neighborNode));
	n->vertexN = value;
	n->eWeight = weight;
	n->visited = 0;
	return n;
}

// PROTOTYPE
static int getMaxVertex(FILE *fp);
static vertexNode *buildAdjList(FILE *fp, DArray *adjList, int max);
static void updateVertex(void *v, BinomialNode *bn);
static void dijkstra(DArray *adjList, Binomial *pQueue, vertexNode *min);
static void displayForest(queue *visited);
static int compareVertex(void *a, void *b);
static vertexNode *findNeighborVertex(DArray *adjList, neighborNode *n);
static void displayVertex(FILE *fp, void *val);
//END

int main(int argc, char *argv[]) {
	FILE *fp = fopen(argv[1], "r");
	FILE *fp2 = fopen(argv[1], "r");
	int justsupressingshit = argc;
	justsupressingshit = 0;
	//queue *visitedQ = newQueue(displayInteger);
	vertexNode *min;

	int max = getMaxVertex(fp)+1;

	// create a adjList the size of the largest Vertex in the file
	DArray *adjList = newDArray(displayInteger);
	for(int i = 0; i < max; i++) {
		insertDArray(adjList, NULL);
}

	// re-read the file and insert your vertexNodes and neighborNodes into the adjList
	// and return the min vertexNode
	min = buildAdjList(fp2, adjList, max);


   	Binomial *pQueue = newBinomial(displayVertex, compareVertex, updateVertex);
	
	for(int i = 0; i < sizeDArray(adjList); i++) {
          vertexNode *v = getDArray(adjList, i);
          v->position = insertBinomial(pQueue, v);
     }


    dijkstra(adjList, pQueue, min);

	return 0;
}

//
//	PRIVATE FUNCTIONS
//

void dijkstra(DArray *adjList, Binomial *pQueue, vertexNode *min) {
    queue *visited = newQueue(displayVertex);
    min->distance = 0;
    min->prev = min;
    min->steps =  0;
    while(sizeBinomial(pQueue) != 0) {
          vertexNode *u = (vertexNode *) extractBinomial(pQueue);
          // If we hit the end of a tree in the forest, print the tree then
          // continue as normal.
          if(u->prev == NULL) {
               u->distance = 0;
               displayForest(visited);
               visited = newQueue(displayVertex);
          }
          enqueue(visited, u);
          u->visited = 1;
          int i = 0;
          for(i = 0; i < sizeDArray(u->neighborList); i++) {
               neighborNode *n = (neighborNode *) getDArray(u->neighborList, i);
               vertexNode *v = findNeighborVertex(adjList, n);
               if(v->visited != 1) {
                    int distance = u->distance + n->eWeight;
                    if(distance < v->distance) {
                         v->prev = u;
                         v->distance = distance;
                         v->steps = v->prev->steps + 1;
                         decreaseKeyBinomial(pQueue, v->position, v);
                    }
               }
          }
     }
     // Make sure to print out the final tree after the alg. is done.
     displayForest(visited);
}
static void displayForest(queue *visited) {
     if(sizeQueue(visited) == 0) return;

     Binomial *b = newBinomial(displayVertex, compareVertex, updateVertex);
     int s = sizeQueue(visited);
     int maxSteps = 0;

     int i = 0;
     for(i = 0; i < s; i++) {
          vertexNode *v =  (vertexNode *) dequeue(visited);
          if(v->steps > maxSteps) maxSteps = v->steps;
          insertBinomial(b, v);
     }
     queue *level[maxSteps];
     for(i = 0; i <= maxSteps; i++) {
          level[i] = newQueue(displayVertex);
     }

     for(i = 0; i < s; i++) {
          vertexNode *v = (vertexNode *) extractBinomial(b);
          enqueue(level[v->steps], v);
     }
     for(i = 0; i <= maxSteps; i++) {
          fprintf(stdout, "%d : ", i);
          while(sizeQueue(level[i]) != 0) {
               vertexNode *v = (vertexNode *) dequeue(level[i]);
               displayVertex(stdout, v);
               if(sizeQueue(level[i]) > 0) fprintf(stdout, " ");
          }
          fprintf(stdout, "\n");
     }
     fprintf(stdout, "----\n");
}
// void displayForest(DArray *nodes){ //add file pointer
// 	if(sizeDArray(nodes) == 0){
// 		//printf("0:\n");
// 		return;
// 	}
// 	int maxSteps = 0;
// 	for(int i=0; i<sizeDArray(nodes); i++){
// 		vertexNode *getMax = (vertexNode *) getDArray(nodes, i);
// 		if(getMax->steps > maxSteps){
// 		maxSteps = getMax->steps;
// 		}
// 	}
// 	queue *level[maxSteps];
// 	for(int j=0; j<=maxSteps; j++){
// 		level[j] = newQueue(displayVertex);
// 	}

// 	for(int k=0; k<sizeDArray(nodes); k++){
// 		vertexNode *v = (vertexNode *) getDArray(nodes, k);
// 		enqueue(level[v->steps], v);
// 	}

// 	for(int a=0; a<=maxSteps; a++){
// 		printf("%d : ", a);
// 		while(sizeQueue(level[a]) != 0){
// 			vertexNode *print = (vertexNode *) dequeue(level[a]);
// 			displayVertex(stdout, print);
// 			if(sizeQueue(level[a]) > 0)
// 				fprintf(stdout, " "); 
// 		}
// 		fprintf(stdout, "\n");
// 	} 
// 	fprintf(stdout, "----\n");
// }

void updateVertex(void *v, BinomialNode *bn) {
	((vertexNode *) v)->position = bn;
}

vertexNode *vertInList(DArray *list, int vert) {
    for(int i = 0; i < sizeDArray(list); i++) {
        vertexNode *v = getDArray(list, i);
        if(v->vertex == vert) {
            return v;
        }
    }
    return NULL;
}

vertexNode * findNeighborVertex(DArray *adjList, neighborNode *n) {
     vertexNode *v = vertInList(adjList, n->vertexN);
     return v;
}

void displayVertex(FILE *fp, void *val) {
     vertexNode *v = (vertexNode *) val;
     fprintf(fp, "%d", v->vertex);
     if(v->prev != NULL && v->prev != v) {
          fprintf(fp, "(%d)", v->prev->vertex);
          fprintf(fp, "%d", v->distance);
     }
     return;
}

int compareVertex(void *a, void *b) {
     if(a == NULL && b == NULL) return 0;
     else if(a == NULL) return -1;
     else if(b == NULL) return 1;

     vertexNode *x = (vertexNode *) a;
     vertexNode *y = (vertexNode *) b;
     if(x->distance == y->distance) {
          if(x->vertex < y->vertex) {
               return -1;
          } else {
               return 1;
          }
     } else if(x->distance < y->distance) {
          return -1;
     } else {
          return 1;
     }
}

int getMaxVertex(FILE *fp) {
	double max = -9999999;
	int v1 = readInt(fp);
	int v2 = readInt(fp);
	char *edgeStr = readToken(fp);
	if((strcmp(edgeStr, ";")) != 0) {
		edgeStr = readToken(fp);
	}

	while(!feof(fp)) {
		if(v1 > max) max = v1;
		if(v1 > max) max = v2;
		if((strcmp(edgeStr, ";")) != 0) {
			edgeStr = readToken(fp);
		}
		v1 = readInt(fp);
		v2 = readInt(fp);
		edgeStr = readToken(fp);
	}
		return max;
}

vertexNode *buildAdjList(FILE *fp, DArray *adjList, int max) {
	double min = INT_MIN;
	int v1, v2,w;
	char *edgeStr;

	v1 = readInt(fp);
	v2 = readInt(fp);
	edgeStr = readToken(fp);

	while(!feof(fp)) {

		// find the small value of the two vertices
		if(v1 < min) min = v1;
		if(v2 < min) min = v2;	

		// determine value of w(weight);
		if(!(strcmp(edgeStr, ";"))) w = 1;
		else {
			w = atoi(edgeStr);
			edgeStr = readToken(fp);
		}

		vertexNode *vertex = newVertexNode(v1);	
		neighborNode *neighbor = newNeighborNode(v2,w);

		// check if the vertex exist and if not populate the neighborList with NULL and add the neighborNode
		// else, check if the neighbor exist and if so compare the weight to the old one and replace if necessary
		// else, create neighbor
		if(getDArray(adjList, v1) == NULL) {
			// since the vertex doesn't exist yet, populate it's neighborList with NULL
			for(int i = 0; i < max; i++) {
				insertDArray(vertex->neighborList, NULL);
			}
			// Now, place the neighborNode in it's corresponding index inside of neighborList
			setDArray(vertex->neighborList, v2, neighbor);
			// place it into the adjList
			setDArray(adjList, v1, vertex);
		}
		else {
			vertexNode *tempVertex = getDArray(adjList, v1);
			if(getDArray(tempVertex->neighborList, v2) != NULL) {
				neighborNode *tempNeighbor = getDArray(tempVertex->neighborList, v2);
				if(tempNeighbor->eWeight > w) tempNeighbor->eWeight = w;
			}
			else setDArray(tempVertex->neighborList, v2, neighbor);
		}

		// get next value of each
		v1 = readInt(fp);
		v2 = readInt(fp);
		edgeStr = readToken(fp);

	}	
	//return the minimum vertex in the adjList
	return getDArray(adjList, min);
}

//
// END 
//
