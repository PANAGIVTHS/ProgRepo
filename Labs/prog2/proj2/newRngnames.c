
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void randomizeStr (char **word) {
    int size;

    for (size = 1; (*word)[size] != '\0'; size++);
    for (int i = 0; i < size; i++) {
        (*word)[i] = (((((*word)[i]) >> 1) * (int)(*word)[i+1]) << 2) % 24 + 'a';  
    }
    
}


void scramble(char *p, int len) {
    int i = 0;

    while(i < len) {
        int c1 = (rand() % (len));
        int c2 = (rand() % (len));

        if(c1 != c2) {
            p[c1] = p[c1] ^ p[c2];
            p[c2] = p[c1] ^ p[c2];
            p[c1] = p[c1] ^ p[c2];

            i++;
        }
    }
}

int main (int argc, char *argv[]) {
    char *word;
    long unsigned int i = 0;

    srand( (unsigned)time(NULL) );
    
    do {
        scanf("%ms", &word);
        if (strcmp(word, "q")==0) break;
        printf("a %lu Name %s 1\n", 200000+i, word);
        i++;
    } while (1);

    return 0;
}

