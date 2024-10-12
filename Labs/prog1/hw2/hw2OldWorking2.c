
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hw2.h"

void lower(char *word) ;
int read_synonyms(char *filename, char synonyms[][MAX_WORD_LEN*2]) ;

// Function returns -1 if the str does not exist 
// or returns the position of the array in which it found the word
int doesStrExist (char wordArray[][MAX_WORD_LEN], char strSearchKey[], int wordsWritten) {

    for (int k = 0; k < wordsWritten; k++) {
        if (strcmp(wordArray[k], strSearchKey) == 0) {
            return(k);
        }
    }
    return(-1);
}

int testforNULL (char* ptrWordArray[][2],int variable, int index) {
    if (variable<MAX_WORDS) { 
        //Avoid scanning the whole array in case of EOF
        if (ptrWordArray[variable][index] == NULL) return(1);
    }
    return(0);
}

void scanText (char wordArray[][MAX_WORD_LEN], char* ptrWordArray[][2]) {
    char formatStr[24], temp[MAX_WORD_LEN] = {'\0'};
    sprintf(formatStr, "%%%ds", MAX_WORD_LEN-1);
    int doesExist;

    //j is counter, i is words written
    for (int j = 0, i = 0; j<MAX_WORDS; j++) {
        
        // When j = MAX_WORDS ptrWordArray is for sure out of bounds
        if (scanf(formatStr, temp) != EOF) {
            doesExist = doesStrExist(wordArray, temp, i);
            printf("%d",doesExist);
            if (doesExist != -1) {
                ptrWordArray[j][0] = wordArray[doesExist];
            }
            else {
                ptrWordArray[j][0] = wordArray[i];
                strcpy(wordArray[i], temp);
                i++;
            }
        }
    }
}

void printText (char *ptrWordArray[][2], int linelen) {
    int nextToUpper = 0, length = 0;
    
    printf("\n");
    for (int i = 0; i < MAX_WORDS; i++) {

        // put in for
        if (testforNULL(ptrWordArray, i ,0)) break;

        if(nextToUpper && (i != MAX_WORDS-1)) {
            ptrWordArray[i][0][0] = toupper(ptrWordArray[i][0][0]);
            if (testforNULL(ptrWordArray, i ,1) == 0) {
                ptrWordArray[i][1][0] = toupper(ptrWordArray[i][1][0]);
            }
            nextToUpper = 0;
        }
        if(strcmp(ptrWordArray[i][0], ".") == 0 || strcmp(ptrWordArray[i][0], "?") == 0 || strcmp(ptrWordArray[i][0], "!") == 0) {
            nextToUpper = 1;
        }
        
        testforNULL(ptrWordArray, i ,1) ? (length += (strlen(ptrWordArray[i][0]) + 1)) : (length += (strlen(ptrWordArray[i][1]) + strlen(ptrWordArray[i][0]) + 4));
        // kati la8os edw
        // printf("-%d-", length);
        if (length >= linelen) {
            testforNULL(ptrWordArray, i, 1)? printf("\n%s ", ptrWordArray[i][0]) : (printf("\n%s (%s) ", ptrWordArray[i][1], ptrWordArray[i][1])); 
            testforNULL(ptrWordArray, i ,1) ? (length = (strlen(ptrWordArray[i][0]) + 1)) : (length = (strlen(ptrWordArray[i][1]) + strlen(ptrWordArray[i][0]) + 4));
        }
        else {
            testforNULL(ptrWordArray, i, 1)? (printf("%s ", ptrWordArray[i][0])) : (printf("%s (%s) ",ptrWordArray[i][0], ptrWordArray[i][1]));
        }
        if(i != MAX_WORDS-1) {
            ptrWordArray[i][0][0] = tolower(ptrWordArray[i][0][0]);
            if (testforNULL(ptrWordArray, i ,1) == 0) {
                ptrWordArray[i][1][0] = tolower(ptrWordArray[i][1][0]);
            }
        }
    }
    printf("\n");
}

void findFrequency (char *ptrWordArray[][2], int frequencyArray[]) {
    // newDistance = -1 for the first case of the distance of pointer j=0: ptrWordArray[j]-ptrWordArray[0]/2 = 0 = oldDistance
    // need -1 to initialize
    int newDistance = 0, oldDistance;
    
    // k is the position of the first pointer in ptrWordArray of diffrent value
    for (int i = 0, k = 0; i < MAX_WORDS && k<MAX_WORDS; i++) {
        if (testforNULL(ptrWordArray, i, 0)) break;

        // start searching from the previously found pointer 
        for (int j = k; j<MAX_WORDS; j++) {
            if (testforNULL(ptrWordArray, k, 0)) break;
            if (testforNULL(ptrWordArray, j, 0)) break;

            if (strcmp(ptrWordArray[k][0],ptrWordArray[j][0]) == 0) {
                //printf("%d|%d-->Test%s\n", k,j,ptrWordArray[k][0]);
                frequencyArray[(ptrWordArray[j][0] - ptrWordArray[0][0])/24]++;
            }
        }
        // To differentiate the pointers we just use the distance between the index (of the array) to which they are pointing and index 0
        oldDistance = newDistance;
        for (; k<MAX_WORDS; k++) {
            if (testforNULL(ptrWordArray, k, 0)) break;
            // Find the next pointer
            if ((ptrWordArray[k][0] - ptrWordArray[0][0])/24 > oldDistance) {
                //printf("Yes %ld.", (ptrWordArray[k][0] - ptrWordArray[0][0])/24);
                newDistance = (ptrWordArray[k][0] - ptrWordArray[0][0])/24;
                //printf("New : %d>%d\n", newDistance, oldDistance);
                break;
            } 
            //printf(">%d ", k);
        }
    }
}

int findArrayMax (int frequencyArray[]) {
    // FreqArray has positive values
    int currentMax = -1;
    int indexesSum = 0;

    for (int i = 0; i < MAX_WORDS && (frequencyArray[i] != 0); i++) {
        if (currentMax < frequencyArray[i]) {
            currentMax = frequencyArray[i];
        }
        indexesSum += frequencyArray[i];
        // Avoid searching the whole array
        if (currentMax >= MAX_WORDS - currentMax) {
            return(currentMax);
        }
    }
    return(currentMax);
}

int findFrequentWords (char wordArray[][MAX_WORD_LEN], char *ptrWordArray[][2], char mostFreqWordsArray[][MAX_WORD_LEN]) {
    int frequencyArray[MAX_WORDS] = {0};
    int indexesSum = 0;
    int k = 0, currentMax;

    findFrequency (ptrWordArray, frequencyArray);
    currentMax = findArrayMax (frequencyArray);

    for (int i = 0; (i < MAX_WORDS) && (wordArray[i][0] != '\0') && (currentMax < MAX_WORDS - indexesSum); i++) {
        indexesSum += frequencyArray[i];
        if (currentMax == frequencyArray[i]) {
            strcpy(mostFreqWordsArray[k], wordArray[i]);
            k++;
        }
    }
    return(currentMax);
}

void printWords (char wordArray[][MAX_WORD_LEN]) {
    for (int i = 0; i < MAX_WORDS && (wordArray[i][0] != '\0'); i++) {
        printf("%s\n", wordArray[i]);
    }
}

void findSynonyms (char *ptrWordArray[][2], char synonymsArray[][MAX_WORD_LEN*2]) {
    // used to stop searching when out of synonyms
    int usedSynonyms[MAX_WORDS] = {0};
    int outOfSynonyms;
    
    for (int i = 0; i < MAX_WORDS && (testforNULL(ptrWordArray, i, 0) == 0); i++) {
        outOfSynonyms = 0;
        for (int j = 0; j < MAX_WORDS && (synonymsArray[j][0] != '\0'); j++) {
            // if true there are no more synonyms
            for (int k = 0; k < MAX_WORDS; k++) {
                if (usedSynonyms[k] == 0) break;
                else if (k == MAX_WORDS - 1){
                    outOfSynonyms = 1;
                }
            }
            if ((usedSynonyms[j] == 0) && (ptrWordArray[i][0][0] == synonymsArray[j][0]) && (strcmp(ptrWordArray[i][0], synonymsArray[j]) == 0)) {
                // maybe error
                ptrWordArray[i][1] = &synonymsArray[j][strlen(synonymsArray[j]) + 1];
                for (int k = i+1; k<MAX_WORDS && (testforNULL(ptrWordArray, k, 0) == 0); k++) {
                    // run ptrWordArray from current pointer and find the same ones
                    // then set same address  
                    if (ptrWordArray[k][0] == ptrWordArray[i][0]) {
                        ptrWordArray[k][1] = ptrWordArray[i][1]; 
                    }
                }
                usedSynonyms[j] = 1;
            }
            // if true it means you have run out of words to find synonyms for
            if (testforNULL(ptrWordArray, i+1, 0) || outOfSynonyms) break;
        }
    }
}

int read_synonyms(char *filename, char synonyms[][MAX_WORD_LEN*2]) {
    int i;
    char word[MAX_WORD_LEN]={'\0'}, word_synonym[MAX_WORD_LEN]={'\0'};
    char format[30];
    FILE *fp;
  
    fp = fopen(filename, "r");
    if(!fp) {
        fprintf(stderr, "Error: Could not open file \"%s\" for reading\n", filename);
        return 0;
    }
  
    sprintf(format, "%%%ds", MAX_WORD_LEN-1);
    memset(synonyms, 0, MAX_SYNONYMS*MAX_WORD_LEN*2);  

    i=0;
    while(i < MAX_SYNONYMS && EOF != fscanf(fp, format, word)) {
        lower(word);
        strcpy(synonyms[i], word);
        if (EOF == fscanf(fp, format, word_synonym))  {   
            fclose(fp);
            fprintf(stderr, "Error: Missing synonym for word \"%s\"\n", word);
            return 0;
        }
        lower(word_synonym);
        strcpy(synonyms[i]+strlen(word)+1, word_synonym);
        i++;
    }
    fclose(fp);
    return 1;
}

void lower(char *word) {
    while (*word) {
        *word = tolower(*word);
        word++;
    }
}

int main(int argc, char *argv[]) {
    int runR=0, runP=0, runM=0;
    char wordArray[MAX_WORDS][MAX_WORD_LEN] = {'\0'};
    char *ptrWordArray[MAX_WORDS][2] = {NULL};
    char mostFreqWordsArray[MAX_WORDS][MAX_WORD_LEN] = {'\0'};
    char synonymsArray[MAX_WORDS][MAX_WORD_LEN*2] = {{'a','\0','b','\0'},{'g','\0','h','\0'}};
    
    for (int i = 1; i < argc; i++) {
        // i=1 cause argv[1] is ./hw2
        if (strcmp(argv[i], "-r") == 0) runR = i;
        if (strcmp(argv[i], "-m") == 0) runM = i;
        if (strcmp(argv[i], "-p") == 0) runP = i;
        if ((runM != 0) && (runP != 0) && (runR !=0)) break;
    }
    /* Same as 3 OR */
    if (runM+runP+runR == 0) {
        printf("Incorrect command-line arguments!\n");
        return(0); // Should return 1 tho. No?
    }

    if (runR != 0 && read_synonyms(argv[runR+1], synonymsArray)) {
        // do stuff
    }
    printf("Waiting...\n");
    scanText(wordArray, ptrWordArray);
    // We know \0 exists. We use the function 
    printf(">>%d ", doesStrExist(wordArray, "\0", MAX_WORDS));
    //printf("%s \n", ptrWordArray[1][0]);
    findSynonyms(ptrWordArray, synonymsArray);
    //printf("%c",toupper(ptrWordArray[1][0][0]));
    printText(ptrWordArray, 24);
    printf("Max: %d\n", findFrequentWords(wordArray, ptrWordArray, mostFreqWordsArray));
    printWords(mostFreqWordsArray);
    printf("wait");
    //printf("%ld\n", (ptrWordArray[5] - ptrWordArray[0])/2);
    //printf("%p|%p", ptrWordArray[5] , ptrWordArray[0]);
    return(0);
    
}