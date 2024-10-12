/*
	This programme takes words, numbers as arguments. It sorts the numbers & it removes a given character
	from every word given
 */

#include"lab8.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int strRemove (char *string, char letter);
int handleArguments (int numOfarguments, char *argumentsStr[], int numArray[], int *ptrWordCounter, char wordArray[][WORD_WIDTH], int *ptrNumCounter);
void swap (int *array, int index1, int index2);
void sortNumArray (int numArray[], int numCounter);
void printArray (int numArray[], int numCounter);
int removeElement (char wordArray[][WORD_WIDTH], int wordCounter, char inputLetter);
void printWords (char wordArray[][WORD_WIDTH], int wordCounter);

int main (int argc, char *argv[]) {
	int numArray[MAX_NUMBERS] = {0};
	char wordArray[MAX_WORDS][WORD_WIDTH] = {0}, inputLetter;
	int *ptrWordCounter = NULL;
	int *ptrNumCounter = NULL;
	int argumentStatus, numCounter, wordCounter;

	ptrNumCounter = &numCounter;
	ptrWordCounter = &wordCounter;

	// if there's a non valid argument return 1
	argumentStatus = handleArguments(argc, argv, numArray, ptrWordCounter, wordArray, ptrNumCounter);
	if (argumentStatus == -1) {
		printf("Incorrect arguments.\n");
		return(1);
	}

	// Sort and Print numbers
	sortNumArray(numArray, numCounter);
	printArray (numArray, numCounter);

	// Remove character & print words
	printf("##\nChar:\n");
	scanf(" %c", &inputLetter);	
	printf("Removed: %d\n", removeElement(wordArray, wordCounter, inputLetter));
	printWords(wordArray, wordCounter);

	return(0);
}

int handleArguments (int numOfarguments, char *argumentsStr[], int numArray[], int *ptrWordCounter, char wordArray[][WORD_WIDTH], int *ptrNumCounter) {
	
	int numCounter = 0;
	int wordCounter = 0;

	// check if out of arguments
	for (int i = 1; i < numOfarguments && (*argumentsStr[i] != '\0'); i++) {	
		
		// Check if current string is the "-word" and store the word given
		if ((strcmp(argumentsStr[i], "-word") == 0) && (i%2 == 1)) { /*i%2 in case of repeating arguments ex. (-word -word -word ..)*/
			if ((wordCounter < MAX_WORDS) && (i+1 < numOfarguments)) {
				strncpy(wordArray[wordCounter], argumentsStr[i + 1], WORD_WIDTH-1);
				wordCounter++;
			}
			// string is "-word" but reached MAX_WORDS
			else continue;
		}
		// Check if current string is the "-number" and store the number given
		else if ((strcmp(argumentsStr[i], "-number") == 0) && (i%2 == 1) && (i+1 < numOfarguments)) { /*i%2 for repeating arguments.. */
			if (numCounter < MAX_NUMBERS) {
				numArray[numCounter] = atoi(argumentsStr[i + 1]);
				numCounter++;
			}
			else continue;
		}
		// if string isn't an argument (or word, number) return Error.
		else if ((strcmp(argumentsStr[i-1], "-number") != 0) && (strcmp(argumentsStr[i-1], "-word") != 0)) {
			return(-1);
		}
	}
	// Set output values for pointers
	*ptrWordCounter = wordCounter;
	*ptrNumCounter = numCounter;
	return(0);
}

/*Swap two indexes*/
void swap (int *array, int index1, int index2) {
	int temp;

	temp = array[index1];
	array[index1] = array[index2]; 
	array[index2] = temp;

}

void sortNumArray (int numArray[], int numCounter) {
	int i = numCounter-1, j = 0;
	while (1) {
		// move right until current number < first number in array
		for (; i >= 0 && (numArray[0] < numArray[i]); i--);
		// move left until current number > first number in array
		for (; j < numCounter && (numArray[j] <= numArray[0]); j++);
		// if the pointers overlap break
		if (i - j <= 0) break;
		swap (numArray, i ,j);
		// start one over the previous index (j one to right, i one to left)
		j++;
		i--;
	}
} 

void printArray (int numArray[], int numCounter) {
	// Print the sorted array's numbers
	for (int i = 0; i < numCounter; i++) {
		i != numCounter - 1? (printf("%d ", numArray[i])) : (printf("%d\n", numArray[i]));
	}
}

int removeElement (char wordArray[][WORD_WIDTH], int wordCounter, char inputLetter) {
	char *ptrCounterI;
	ptrCounterI = &wordArray[0][0];
	int removedElementsNum = 0;


	// Run the array using pointers
	// one row down (+= WordWidth)
	for (; ((ptrCounterI - &wordArray[0][0])/WORD_WIDTH) < wordCounter; ptrCounterI += WORD_WIDTH) {
		if (strchr(ptrCounterI, inputLetter) == NULL) continue;
		removedElementsNum += strRemove(ptrCounterI, inputLetter);
	}
	return(removedElementsNum);
}

// --Edited this function: epeidh den jerw na diabazw ekfwnhseis.--

// reorder the word putting all 'letter' characters last
// and deleting them all at once. This way you only check the whole
// the array once (worst case)

// 'Reorder' by replacing each 'letter' char with the next non-'letter' one
int strRemove (char *string, char letter) {
	char *pos;
	int elementsDeleted;

	// Char doesnt exist in string
	if (string == NULL) return(0);
	string = strchr(string, letter);
	
	for(int i = 0; 1 ; i++) {
		// find pos of first 'letter' in string
		pos = strchr(string, letter);
		// if you find '\0' then you are done reordering
		if ((string[i] != letter) && (string[i] == '\0')) {
			// delete all 'letter' characters up to '\0'
			for (elementsDeleted = 0; *pos != '\0'; pos++,elementsDeleted++) {
				*pos = '\0';
			}
			return(elementsDeleted);
		}
		else {
			// swap 'letter' with the first non-'letter' character
			*pos = string[i];
			string[i] = letter; 
		}
	}
	return(0);
}

void printWords (char wordArray[][WORD_WIDTH], int wordCounter) {
	for (int i = 0; i < wordCounter; i++) {
		printf("%s\n", wordArray[i]);
	}
}
