
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hw2.h"

#define NUM_RUN_PARAMETERS 4
#define MAX_PARAMETERS_LENGTH 3

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

void scanText (char wordArray[][MAX_WORD_LEN], char* ptrWordArray[][2]) {
    char formatStr[25], temp[MAX_WORD_LEN] = {'\0'};
    sprintf(formatStr, "%%%ds", MAX_WORD_LEN-1);
    int doesExist;

    //j is counter, i is words written
    for (int j = 0, i = 0; (j<MAX_WORDS) && (i < MAX_WORDS); j++) {
        
        // When j = MAX_WORDS ptrWordArray is for sure out of bounds
        if (scanf(formatStr, temp) != EOF) {
            //printf("string : %s\n ", temp);
            lower(temp);   
            doesExist = doesStrExist(wordArray, temp, i);
            //printf("de: %d ", doesExist);
            if (doesExist != -1) {
                ptrWordArray[j][0] = wordArray[doesExist];
            }
            else {
                ptrWordArray[j][0] = wordArray[i];
                strcpy(wordArray[i], temp);
                i++;
            }
            //printf("in scan %d", j);
            //printf("\n i in scan %d\n", i);
        }
    }
}

void printText (char *ptrWordArray[][2], int linelen) {
    int nextToUpper = 0, length = 0;
    
    printf("\n");
    for (int i = 0; i < MAX_WORDS && (ptrWordArray[i][0] != NULL); i++) {

        if((nextToUpper || i==0) && (i != MAX_WORDS-1)) {
            ptrWordArray[i][0][0] = toupper(ptrWordArray[i][0][0]);
            if (ptrWordArray[i][1] != NULL) {
                ptrWordArray[i][1][0] = toupper(ptrWordArray[i][1][0]);
            }
            nextToUpper = 0;
        }
        if(strcmp(ptrWordArray[i][0], ".") == 0 || strcmp(ptrWordArray[i][0], "?") == 0 || strcmp(ptrWordArray[i][0], "!") == 0) {
            nextToUpper = 1;
        }
        
        ptrWordArray[i][1] == NULL ? (length += (strlen(ptrWordArray[i][0]) + 1)) : (length += (strlen(ptrWordArray[i][1]) + strlen(ptrWordArray[i][0]) + 4));
        // kati la8os edw
        if (length > linelen && (ptrWordArray[i][1] != NULL) && (length - strlen(ptrWordArray[i][1]) -3 > linelen)) {
            //printf("-%d-", length);
        
            length = strlen(ptrWordArray[i][1]) + strlen(ptrWordArray[i][0]) + 4;
            printf("\n%s (%s) ", ptrWordArray[i][0], ptrWordArray[i][1]);
            
        }
        else if (length > linelen && (ptrWordArray[i][1] == NULL) && (length > linelen)) {
            printf("\n%s ", ptrWordArray[i][0]);
            length = strlen(ptrWordArray[i][0]) + 1;
        }
        else if ((length > linelen) && (ptrWordArray[i][1] != NULL)) {
            printf("%s\n(%s) ", ptrWordArray[i][0], ptrWordArray[i][1]);
            length = strlen(ptrWordArray[i][1]) + 3;
        }
        else {
            ptrWordArray[i][1] == NULL ? (printf("%s ", ptrWordArray[i][0])) : (printf("%s (%s) ",ptrWordArray[i][0], ptrWordArray[i][1]));
        }
        if(i < MAX_WORDS) {
            ptrWordArray[i][0][0] = tolower(ptrWordArray[i][0][0]);
            if (ptrWordArray[i][1] != NULL) {
                ptrWordArray[i][1][0] = tolower(ptrWordArray[i][1][0]);
            }
        }
        //printf("in print %d\n", i);
    }
    printf("\n");
}

void findFrequency (char *ptrWordArray[][2], int frequencyArray[]) {
    int newDistance = 0, oldDistance;
    
    // k is the position of the first pointer in ptrWordArray of diffrent value
    for (int i = 0, k = 0; i < MAX_WORDS && k<MAX_WORDS; i++) {
        if (ptrWordArray[i][0] == NULL) break;

        // start searching from the previously found pointer 
        for (int j = k; j<MAX_WORDS && (ptrWordArray[j][0] != NULL); j++) {
            if (ptrWordArray[k][0] == NULL) break;

            if (strcmp(ptrWordArray[k][0],ptrWordArray[j][0]) == 0) {
                //printf("%d|%d-->Test%s\n", k,j,ptrWordArray[k][0]);
                frequencyArray[(ptrWordArray[j][0] - ptrWordArray[0][0])/MAX_WORD_LEN]++;
            }
        }
        // To differentiate the pointers we just use the distance between the index (of the array) to which they are pointing and index 0
        oldDistance = newDistance;
        for (; k<MAX_WORDS; k++) {
            if (ptrWordArray[k][0] == NULL) break;
            // Find the next pointer
            if ((ptrWordArray[k][0] - ptrWordArray[0][0])/MAX_WORD_LEN > oldDistance) {
                //printf("Yes %ld.", (ptrWordArray[k][0] - ptrWordArray[0][0])/MAX_WORD_LEN);
                newDistance = (ptrWordArray[k][0] - ptrWordArray[0][0])/MAX_WORD_LEN;
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
        // currentMax >= MAX_WORDS - indexesSum : The sum of all numbers must be MAX_WORDS
        // Therefore if currentMax >= remainingTotal then the max = currentMax
        if (currentMax >= MAX_WORDS - indexesSum) {
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

    // currentMax < MAX_WORDS - indexesSum : The sum of all numbers must be MAX_WORDS
    // Therefore if currentMax >= remainingTotal then no more 'Maxes' to be found
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
    // Prints all words (one at a time) in a given array
    for (int i = 0; i < MAX_WORDS && (wordArray[i][0] != '\0'); i++) {
        printf("%s\n", wordArray[i]);
    }
}

/*
    refrence name for ptrWordArray : p[][]
    Description: Find the word from wordArray that ptrWordArray[n][0] ( = p[n][0]) is pointing at. Find
    the value for ptrWordArray[n][1]. Find all pointers (ptrWordArray[x][0]) that point to
    the same address as p[n][0] (same string) and copy the value from p[n][1] to p[x][1]
*/
void findSynonyms (char *ptrWordArray[][2], char synonymsArray[][MAX_WORD_LEN*2]) {
    // used to stop searching when out of synonyms
    int usedSynonyms[MAX_SYNONYMS] = {0};
    int outOfSynonyms;
    int newDistance=-1 ,oldDistance;
    
    for (int i = 0; i < MAX_WORDS && (ptrWordArray[i][0] != NULL); i++) {
        outOfSynonyms = 0;
        oldDistance = newDistance;
        

        // Find the next pointer (Dont run same word twice)
        if ((ptrWordArray[i][0] - ptrWordArray[0][0])/MAX_WORD_LEN > oldDistance) {
            newDistance = (ptrWordArray[i][0] - ptrWordArray[0][0])/MAX_WORD_LEN;

            for (int j = 0; j < MAX_SYNONYMS && (synonymsArray[j][0] != '\0'); j++) {
                // if outOfSynonyms is true there are no more synonyms
                for (int k = 0; k < MAX_SYNONYMS; k++) {
                    if (usedSynonyms[k] == 0) break;
                    else if (k == MAX_SYNONYMS - 1){
                        outOfSynonyms = 1;
                    }
                }
                // Dont check for the same synonym.
                //printf("|%d",  usedSynonyms[j]);
                if ((usedSynonyms[j] == 0) && (strcmp(ptrWordArray[i][0], synonymsArray[j]) == 0)) {
                    // Get the adress of the synonym word
                    //printf(" yes ");
                    ptrWordArray[i][1] = &synonymsArray[j][strlen(synonymsArray[j]) + 1];
                    for (int k = i+1; k<MAX_WORDS && (ptrWordArray[k][0] != NULL); k++) {
                        //printf("k: %d",k);
                        // run ptrWordArray from current pointer and find the same ones
                        // then set same address  
                        if (ptrWordArray[k][0] == ptrWordArray[i][0]) {
                            ptrWordArray[k][1] = ptrWordArray[i][1]; 
                        }
                    }
                    usedSynonyms[j] = 1;
                    break;
                }
                // if true it means you have run out of words to find synonyms for
                if (outOfSynonyms) printf("run out");
            }
            //printf("    i:%d p\n",i);
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

long int findMemoryUsage (char wordArray[][MAX_WORD_LEN], char *ptrWordArray[][2]) {
    int numOfEntries, memoryUsage, numOfEntriesPtrArray;

    numOfEntries = (doesStrExist(wordArray, "\0", MAX_WORDS) == -1) ? (MAX_WORDS) : (doesStrExist(wordArray, "\0", MAX_WORDS));
    for (numOfEntriesPtrArray = 0; numOfEntriesPtrArray < MAX_WORDS && (ptrWordArray[numOfEntriesPtrArray][0] != NULL); numOfEntriesPtrArray++);
    // sizeof in case you are running this on a potato (32-bit system)
    memoryUsage = (numOfEntries * MAX_WORD_LEN) + (numOfEntriesPtrArray * sizeof(int*));
    
    return (memoryUsage);
}

long int findTextMemory (char wordArray[][MAX_WORD_LEN], char *ptrWordArray[][2]) {
    int memoryUsage, numOfEntriesPtrArray;

    for (numOfEntriesPtrArray = 0; numOfEntriesPtrArray < MAX_WORDS && (ptrWordArray[numOfEntriesPtrArray][0] != NULL); numOfEntriesPtrArray++);
    // sizeof in case you are running this on a potato (32-bit system)
    memoryUsage = (numOfEntriesPtrArray * MAX_WORD_LEN) + (numOfEntriesPtrArray * sizeof(int*));
    
    return (memoryUsage);
}

double findMemorySavedRatio (char wordArray[][MAX_WORD_LEN], char *ptrWordArray[][2]) {
    return((double)findMemoryUsage(wordArray,ptrWordArray)/(double)findTextMemory(wordArray,ptrWordArray));
}

int main(int argc, char *argv[]) {
    int runR=0, runP=0, runM=0, runS=0;
    char wordArray[MAX_WORDS][MAX_WORD_LEN] = {'\0'};
    char *ptrWordArray[MAX_WORDS][2] = {NULL};
    char mostFreqWordsArray[MAX_WORDS][MAX_WORD_LEN] = {'\0'};
    char synonymsArray[MAX_SYNONYMS][MAX_WORD_LEN*2] = {{0}};
    // for accessibility + expandability
    char runParameters[NUM_RUN_PARAMETERS][MAX_PARAMETERS_LENGTH] = {{"-r\0"},{"-m\0"},{"-p\0"},{"-s\0"}};

    for (int i = 1; i < argc; i++) {
        // i=1 cause argv[1] is ./hw2
        if (strcmp(argv[i], runParameters[0]) == 0) runR = i;
        if (strcmp(argv[i], runParameters[1]) == 0) runM = i;
        if (strcmp(argv[i], runParameters[2]) == 0) runP = i;
        if (strcmp(argv[i], runParameters[3]) == 0) runS = i;
        if ((runM != 0) && (runP != 0) && (runR !=0) && (runS !=0)) break;
    }
    /* Same as OR */
    if (runM+runP+runR+runS == 0) {
        printf("Incorrect command-line arguments!\n");
        return(0); // Should return 1 tho. No?
    }
    scanText(wordArray, ptrWordArray);
    if (runR != 0 && read_synonyms(argv[runR+1], synonymsArray)) {
        findSynonyms(ptrWordArray, synonymsArray);
    }
    if (runM != 0 && (wordArray[0][0] != '\0')) {
        printf("Max: %d\n", findFrequentWords(wordArray, ptrWordArray, mostFreqWordsArray));
        printWords(mostFreqWordsArray);
    }
    if (runP != 0) {
        printText(ptrWordArray, MAX_LINE_LEN);
    }
    
    if (runS != 0 && (wordArray[0][0] != '\0')) {
        printf("%.3lf\n", findMemorySavedRatio(wordArray, ptrWordArray));
    }

    return(0);
}