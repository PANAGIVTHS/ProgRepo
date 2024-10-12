

#include <stdio.h>
#include"lab5.h"
#include <string.h>
#include <ctype.h>

// No need to return as arrays are pointers
void fixUpperLower (char inputArray[]) {
    inputArray[0] = toupper(inputArray[0]);
    for (int i = 1; inputArray[i] != '\0' && i < WORDSIZE; i++) {
        inputArray[i] = tolower(inputArray[i]);
    }
}

int main(int argc, char *argv[]) {

    char exitWordKey[] = {"End-of-words"};
    char inputWord[WORDSIZE] = {'\0'}, inputDictionary[WORDSIZE] = {'\0'}, bestGuess[WORDSIZE] = {'\0'};
    int distance = 0, sortestDistance = 0, firstLoop = 1, sizeOldArray = 0;
    char fstr[16];

    sprintf(fstr, " %%%ds", WORDSIZE-1);
    printf("Word to check:\n");
    scanf(fstr, inputWord);
    printf("Dictionary words:\n");
    fixUpperLower(inputWord);
    do {
        distance = 0;
        for (int i = 0; i < sizeOldArray; i++) {    // Clear array for strcmp
            inputDictionary[i] = '\0';              // needed when the previous string was 
        }                                           // larger than the new one
        scanf(fstr, inputDictionary);
        // Turn 1st letter Upper 2nd - ... Lower
        sizeOldArray = strlen(inputDictionary);
        fixUpperLower(inputDictionary);
        if(strcmp(inputDictionary, exitWordKey) == 0) {
            break;
        }
        // Find distance
        for (int i = 0; (inputDictionary[i] != '\0' || inputWord[i] != '\0') && (i < WORDSIZE); i++) {
            if (inputDictionary[i] != inputWord[i]) {
                distance++;
            }
        }
        if (firstLoop) {
            sortestDistance = distance; // Initialise sortestDistance.
            strcpy(bestGuess, inputDictionary);
            firstLoop = 0;
        }
        if (distance <= sortestDistance) { 
            sortestDistance = distance;
            strcpy(bestGuess, inputDictionary);
        }
        switch(strcmp(inputDictionary, inputWord)) {
            case 0: {
                printf("\"%s\" is correct.\n", inputWord);
                return(0);
            }
            default: {
                printf("\"%s\" distance: %d\n", inputDictionary, distance);
            }
        }
    } while (1);
    if (firstLoop) {
        printf("\"%s\" is not correct.\n", inputWord);
        printf("No suggestion\n");
    }
    else {
        printf("\"%s\" is not correct.\n", inputWord);
        printf("Suggestion: \"%s\" %d\n", bestGuess, sortestDistance);
    }

    return(0);
}