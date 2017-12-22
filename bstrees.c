#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bst.h"
#include "vbst.h"
#include "rbt.h"
#include "input.h"
#include "integer.h"
#include "string.h"
#include "comparator.h"
#include "queue.h"
#include "sll.h"


/*
	Everything is read into a file. This project reads in "strings" or "phrases"
	and handles them according to the project specifications.

	Only thing left is RBT and makefile... I think.
 */

 void readFileBST(FILE *fp, bst *a) {
 	char *str = obtainString(fp);
 	while(!feof(fp)) {
 		stringCleaner(str);
 		if(strcmp(str, "") != 0) insertBST(a, newString(str));
 		str = obtainString(fp);
 	}
 }

 void readFileVBST(FILE *fp, vbst *a) {
 	char *str = obtainString(fp);
 	while(!feof(fp)) {
 		stringCleaner(str);
 		if(strcmp(str, "") != 0) insertVBST(a, newString(str));
 		str = obtainString(fp);
 	}
 }

 void readFileRBT(FILE *fp, rbt *a) {
 	char *str = obtainString(fp);
 	while(!feof(fp)) {
 		stringCleaner(str);
 		if(strcmp(str, "") != 0) insertRBT(a, newString(str));
 		str = obtainString(fp);
 	}
 }
 void readCommands(FILE *fp, queue *q) {
 	char *str = obtainString(fp);
 	while(!feof(fp)) {
 		stringCleaner(str);
 		enqueue(q,newString(str));
 		str = obtainString(fp);
 	}
 }	

int main(int argc, char * argv[]) {

	FILE *fpcommands = fopen(argv[3], "r");
	FILE *fpcorpus = fopen(argv[2], "r");
	FILE *output = stdout;
	if(argc == 5) output =  fopen(argv[4], "w");

	queue *input = newQueue(displayString);
	readCommands(fpcommands, input);
	void *i = newString("i");
	void *s = newString("s");
	void *r = newString("r");
	void *f = newString("f");
	void *d = newString("d");

	if(argv[1][1] == 'v') {
		vbst *a = newVBST(displayString, stringComparator);
		readFileVBST(fpcorpus, a);

		while(sizeQueue(input)>0) {
			void *lastOutput = dequeue(input);
			void *opWord;
			if(stringComparator(lastOutput, i) == 0) {
				opWord = dequeue(input);
				if(stringComparator(opWord, newString("")) != 0) insertVBST(a, opWord);
			}
			else if(stringComparator(lastOutput, s) == 0) {
				displayVBST(output, a);
			}

			else if(stringComparator(lastOutput, r) == 0) {
				statisticsVBST(a, output);
			}
			else if(stringComparator(lastOutput, f) == 0) {
				opWord = dequeue(input);
				fprintf(output, "Frequency of ");
				displayString(output,opWord);
				fprintf(output, ": %d\n", findVBST(a, opWord));
			}
			else if(stringComparator(lastOutput, d) == 0) {
				opWord = dequeue(input);
				deleteVBST(a, opWord);
			}
		}
	}

	else if(argv[1][1] == 'r') {
		rbt *a = newRBT(displayString, stringComparator);
		readFileRBT(fpcorpus, a);

		while(sizeQueue(input)>0) {
			void *lastOutput = dequeue(input);
			void *opWord;
			if(stringComparator(lastOutput, i) == 0) {
				opWord = dequeue(input);

				if(stringComparator(opWord, newString("")) != 0) insertRBT(a, opWord);
			}
			else if(stringComparator(lastOutput, s) == 0) {
				displayRBT(output, a);
			}
			else if(stringComparator(lastOutput, r) == 0) {
				statisticsRBT(a, output);
			}
			else if(stringComparator(lastOutput, f) == 0) {
				opWord = dequeue(input);
				fprintf(output, "Frequency of ");
				displayString(output,opWord);
				fprintf(output, ": %d\n", findRBT(a, opWord));
			}
			else if(stringComparator(lastOutput, d) == 0) {
				opWord = dequeue(input);
				deleteRBT(a, opWord);
			}
		}
	}
	
	return 0;

}