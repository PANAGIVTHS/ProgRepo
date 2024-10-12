#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

char *isTwinStr (char *word) {
    if ((strlen(word) % 2 == 1) || (*word == '\0')) {
        return(NULL);
    }
    else {
        if (strncmp(word, &word[strlen(word)/2], strlen(word)/2) == 0) {
            return(&word[strlen(word)/2]);
        }
        else {
            return(NULL);
        }
    }
    return(NULL);
}

int main (int argc, char *argv[]) {
	
    printf("%p", isTwinStr("abc_abc"));
	return(0);
}
