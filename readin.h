

#ifndef __READIN_INCLUDED__
#define __READIN_INCLUDED__

extern int getMaxVertex(FILE *fp);
extern vertexNode *buildAdjList(FILE *fp, DArray *adjList, int max);


#endif