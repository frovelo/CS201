#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdarg.h>
#include <limits.h>
#include <ctype.h>
#include "readin.h"
#include "integer.h" 
#include "darray.h"
#include "binomial.h"
#include "comparator.h"
#include "scanner.h"
#include "queue.h"
#include "sll.h"

int getMaxVertex(FILE *fp) {
	double max = -INFINITY;
	int v1 = readInt(fp);
	int v2 = readInt(fp);
	char edge = readChar(fp);
	if(edge != ';'){
		edge = readChar(fp);
	}

	while(!feof(fp)) {
		if(first > max) max = first;
		if(second > max) max = second;

		first = readInt(fp);
		second = readInt(fp);
		edge = readChar(fp);
		if(edge != ';') {
			edge = readChar(fp);
		}
	}
		return max;
}

vertexNode *buildAdjList(FILE *fp, DArray *adjList, int max) {
	double min = INFINITY;
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
		if(w != ';') {
			w = 1;
		}
		else w = atoi(edgeStr);

		vertexNode *vertex = newVertexNode(v1);	
		neighborNode *neighbor = newNeighborNode(v2,w);

		// check if the vertex exist and if not populate the neighborList with NULL and add the neighborNode
		// else, check if the neighbor exist and if so compare the weight to the old one and replace if necessary
		// 		else, create neighbor
		if(getDArray(adjList, v1) == NULL) {
			// since the vertex doesn't exist yet, populate it's neighborList with NULL
			for(int i = 0; i < max; i++) {
				insertDArray(vertex->neighborList, NULL);
			}
			// Now, place the neighborNode in it's corresponding index inside of neighborList
			setDArray(v1->neighborList, v2, neighbor);
			// place it into the adjList
			setDArray(adjList, v1, vertex);
		}
		else {
			vertexNode *temp = getDArray(adjList, v1);
			if(getDArray(temp->neighborList, v2) != NULL) {
				neighborNode *tempN = getDArray(temp->neighborList, v2);
				if(temp->eWeight > w) temp->eWeight = w;
			}
			else	setDArray(temp->neighborList, v2, neighbor)
		}

		// get next value of each
		v1 = readInt(fp);
		v2 = readInt(fp);
		edgeStr = readToken(fp);

	}
		return getDArray(adjList, min);
}