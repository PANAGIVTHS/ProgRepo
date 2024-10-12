#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define min(x, y) ( (x<y) ? (x) : (y) )

#define MAX_SIZE 35
#define ALPHABET_SIZE 26
#define NUM_TESTS 7

void init_letter_seq(char letter_seq[], int array_size, int test_number) ;
int numPosToNextChar (char letter_seq[], char currentChar);
void setPtrArray (char letter_sequence[], char *ptrAlpha[]);


int main (int argc, char *argv[]) {
	char letter_sequence[MAX_SIZE] = {'\0'};
    char *ptrAlpha[ALPHABET_SIZE] = {0}; 
    
    if (argv[1] == NULL || (atoi(argv[1]) >= NUM_TESTS) || (atoi(argv[1]) < 0)) {
        printf("Segmentaion fault (core dumped)\n");
        printf("Nah just kidding wrong arguments.\n");
        return(-1);
    }
    init_letter_seq(letter_sequence, sizeof(letter_sequence) + 1, atoi(argv[1]));
    setPtrArray(letter_sequence, ptrAlpha);
    printArray(ptrAlpha);

	return(0);
}

void setPtrArray (char letter_sequence[], char *ptrAlpha[]) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (strchr(letter_sequence, i + 'A') != NULL) {
            ptrAlpha[i] = strchr(letter_sequence, i + 'A');
        }
        else {
            ptrAlpha[i] = NULL;
        }
    }
}

void printArray (char *ptrAlpha[]) {
    char *let_seq_address = NULL;
    int firstChar = 1, position = 0;

    // find letter_seq (the array's address) 
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (ptrAlpha[i] != NULL) {
            let_seq_address = ptrAlpha[i];
            break;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        printf("%c: ", i + 'A');
        if (ptrAlpha[i] == NULL || let_seq_address == NULL) {
            printf("NULL\n");
        }
        else if (firstChar){
            printf("0\n");
            firstChar = 0;
        }
        else {
            printf("%d\n", position);
        }
        if (firstChar == 0 && (ptrAlpha[i] != NULL) && (let_seq_address != NULL)) {
            position += numPosToNextChar(let_seq_address, i + 'A');
        }
    }

}

int numPosToNextChar (char letter_seq[], char currentChar) {
    char *ptrSearch;
    int i = 0;

    ptrSearch = strchr(letter_seq, currentChar);
    for (i = 0; (ptrSearch != NULL) && (*ptrSearch <= currentChar); ptrSearch++, i++);
    
    return(i);
}

/* Arxikopoiei ton pinaka letter_seq me diafores times, analoga me to test_number.
   Parexei NUM_TESTS diaforetika senaria. 
   Sunistatai i timi 35 ('i megaluteri) gia to MAX_SIZE.
   Parametroi: 
	- letter_seq, arxika adeios pinakas xaraktirwn, megethous array_size
	- array_size, to megethos tou pinaka
	- test_number, aukswn arithmos tou epithumitou test, kata protimisi 
					metaksu 1 kai NUM_TESTS (sumperilambanomenwn).
*/
   
void init_letter_seq(char letter_seq[], int array_size, int test_number) {
	int which;
	char tests[NUM_TESTS][36] = { 
		"",  // empty
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ", // all letters, once each
		"AABCCDEFGHIJKLMNNOPPPQRSSSTTUVVWXYZ", // all letters, some duplicates
		"ABCCEEEEEEEEEEFHIKKLNOOQRRRUUXXX", // some letters, some duplicates
		"FGIIIIIIIIIIJKKLMOOPQQRRSTUUVXXXXY", // no letters from beginning of alphabet
		"ZZZZZZZZZZZZZZ",  // single letter, multiple times.
		"BBBCCEFGXY"  // small example, good for debugging
	};
	
	memset(letter_seq, 0, array_size);
	which = test_number % NUM_TESTS;
	strncpy(letter_seq, tests[which], min(array_size, strlen(tests[which])));
}