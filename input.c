#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "input.h"
#include "string.h"
#include "bst.h"

// stole this bitch from stack overflow yeet yeet
 extern void stringCleaner(char *s) {
    char *i = s;
    char *j = s;
    while(*i) {
    	if(isspace((unsigned char)*i)) {
    		while(isspace((unsigned char)*i) || ispunct((unsigned char)*i) || isdigit((unsigned char)*i)) i++; 
    		*j++ = ' ';
    	}
    	else if(!isalpha((unsigned char)*i)) i++;
    	else if(isupper((unsigned char)*i)) {
    		*j++ = tolower((unsigned char)*i);
    		i++;
    	}
    	else if(i == j) {
    		i++;
    		j++;
    	}
    	else *j++ = *i++;
    }
    *j = 0;
}

 extern char *obtainString(FILE *fp) {
 	if(stringPending(fp)){
		return readString(fp);
	}
 	else{
	 	return readToken(fp);
	}
 }
