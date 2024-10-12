/* ONOMATEPWNYMO: TsogkasPanagiotis Nikolaos
AEM: 03672
*/
#include"project1.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

typedef enum {false, true} boolean;

typedef struct {
    unsigned long int comps;
    boolean found;
    int guess;
} findIndexT;

typedef struct {
    char firstName[NAMESIZE];
    char lastName[NAMESIZE];
} studentT;

typedef struct {
    unsigned long int AEM;
    studentT studentID;
    unsigned short int coursesFailed;
} studentEntryT;

typedef struct {
    studentEntryT **entry;
    int size;
    int firstNullEntry;
    boolean isSorted;
} databaseT;

void wordtoUpper (char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = toupper(word[i]);
    }
}

boolean trimWord (char** word, int trimSize) {
    char *tempWord[trimSize];
    int i;

    // PARENTHESIS NEEDED (*word)[i]
    for (i = 0; (*word)[i] != '\0'; i++);
    if (i >= trimSize+1) {
        *tempWord = (char*) realloc(*word, (trimSize+1)*sizeof(char));
        if (tempWord != NULL) {
            word = tempWord;
            (*word)[trimSize-1] = '\0';
        } else {
            return(true);
        }
    }
    wordtoUpper(*word);
    return(false);
}

findIndexT find (databaseT *entriesArray, long unsigned int targetAEM, int indexLimiter) {
    int current_index, prev_index = 0, move_factor = 0;
    findIndexT valueReturn;

    valueReturn.comps = 0;

    if (indexLimiter == 0) {
        current_index = (entriesArray->firstNullEntry)-1;
    } else {
        current_index = indexLimiter;
    }
    
    if (entriesArray->isSorted) {
        // if first index is target
        // found
        if ((entriesArray->entry[0])->AEM == targetAEM) {
            valueReturn.comps++; 
            valueReturn.found = true;
            valueReturn.guess = 0;
            return(valueReturn);
        // if < first index it should go first
        // not found so guess
        } else if ((entriesArray->entry[0])->AEM > targetAEM) {
            valueReturn.comps++; 
            valueReturn.found = false;
            valueReturn.guess = 0;
            return(valueReturn);
        // if last index is target
        // found
        } else if ((entriesArray->entry[current_index])->AEM == targetAEM) {
            valueReturn.comps++; 
            valueReturn.found = true;
            valueReturn.guess = current_index;
            return(valueReturn);
        // if > last index it should remain in it's pos
        // not found so guess
        } else if ((entriesArray->entry[current_index])->AEM < targetAEM) {
            valueReturn.comps++; 
            valueReturn.found = false;
            valueReturn.guess = current_index+1;
            return(valueReturn);
        }
        do {
            // move by move_factor
            move_factor = ceil(fabs((current_index - prev_index)/2.0));
            // move left
            if (((entriesArray->entry[current_index])->AEM) > targetAEM) {
                valueReturn.comps++; 
                prev_index = current_index;
                current_index -= move_factor;
            }
            // move right
            else if (((entriesArray->entry[current_index])->AEM) < targetAEM) {
                valueReturn.comps+=2; 
                prev_index = current_index;
                current_index += move_factor;
            }
            // none of the above means found!
            else {
                valueReturn.comps+=2;
                valueReturn.found = true;
                valueReturn.guess = current_index;
                return(valueReturn);
            }
            // Check if can continue
            if (fabs((double)((current_index - prev_index)/2.0)) < 1) {
                valueReturn.found = false;
                // not found, guess where it should be inserted
                if (targetAEM < entriesArray->entry[current_index]->AEM) {
                    valueReturn.comps++; 
                    valueReturn.guess = current_index;
                }
                else {
                    valueReturn.comps++;
                    valueReturn.guess = current_index+1;
                }
                return(valueReturn);
            }
        } while (1);
    }
    else {
        // Linear serach
        for (int i = 0; (i < entriesArray->size) && (entriesArray->entry[i] != NULL); i++) {
            if ((entriesArray->entry[i])->AEM == targetAEM) {
                valueReturn.comps++; 
                valueReturn.found = true;
                valueReturn.guess = i;
                return(valueReturn);
            }
        }
    }
    // linear search found nothing
    valueReturn.found = false;
    valueReturn.guess = -1;
    return(valueReturn);
}

// clear all memory
void clear (databaseT *entriesArray) {
    for (int i = 0; i < entriesArray->size; i++) {
        free(entriesArray->entry[i]);
    }
    free(entriesArray->entry);
}

// Exit programme
void quit (databaseT *entriesArray) {
    clear(entriesArray);
    exit(0);
}

// A function that does all the work for me :)
// (Check if realloc fails + set new value if the pointer changes)
void doRealloc (databaseT *entriesArray, int appendSize) {
    studentEntryT** tempPtr;
    
    if ((entriesArray->size + appendSize) < 0) return;
    if (appendSize < 0) {
        
        //bug?
        for (int i = 0; i < -appendSize; i++) {
            free(entriesArray->entry[entriesArray->firstNullEntry-1 - i]);
        }
    }
    tempPtr = (studentEntryT**) realloc(entriesArray->entry, (entriesArray->size + appendSize)*sizeof(studentEntryT*));
    if (tempPtr == NULL) {
        return;
    }
    else {
        entriesArray->size += appendSize;
        entriesArray->entry = tempPtr;
    }
}

// Checks if malloc has failed.
void mallocFail (databaseT *entriesArray, int indexToCheck) {
    if (entriesArray->entry[indexToCheck] == NULL) {
        clear(entriesArray);
        exit(43);
    }
}

// Add memory for a student entry. returns true is success false if fails
boolean add (databaseT *entriesArray, studentEntryT studentDummyTemp) {
    studentEntryT* temp;

    if ((find(entriesArray, studentDummyTemp.AEM, 0)).found == false) {
        temp = (studentEntryT*) malloc(sizeof(studentEntryT));
        if (temp == NULL) {
            return(false);
        }
        entriesArray->entry[entriesArray->firstNullEntry] = temp;
        *(entriesArray->entry[entriesArray->firstNullEntry]) = studentDummyTemp;
        entriesArray->firstNullEntry++;
        entriesArray->isSorted = false;
        //success
        return(true);
    }
    // failed
    return(false);
}

// free memory. returns true if success false if it fails
boolean rmv (databaseT *entriesArray, long unsigned int foundAEM, int appendSize) {
    if ((find(entriesArray, foundAEM, 0)).found == true) {
        entriesArray->entry[foundAEM] = entriesArray->entry[(entriesArray->firstNullEntry)-1];
        entriesArray->entry[(entriesArray->firstNullEntry)-1] = NULL;
        (entriesArray->firstNullEntry)--;
        if ((entriesArray->firstNullEntry)%appendSize == 0) {
            doRealloc(entriesArray, appendSize*(-1));
        }
        entriesArray->isSorted = false;

        // success
        return(true);
    }
    // failed
    return(false);
}

// Modify a student's failedCourses number. (if already in database)
boolean mod (databaseT *entriesArray, long unsigned int targetAEM, short unsigned int courses) {
    findIndexT indexFoundIn;
    
    indexFoundIn = find(entriesArray, targetAEM, 0);
    if (indexFoundIn.found != false) {
        (entriesArray->entry[indexFoundIn.guess])->coursesFailed = courses;
        // success
        return(true);
    }
    // failed
    return(false);

}

// Sort students based on AEM number ascending order
// DElete
unsigned long int sort (databaseT *entriesArray) {
    unsigned long int comps = 0;
    int sortedElements = 1;
    studentEntryT *tempPtr;
    findIndexT sortToPos;

    //sortedElements = first unsorted element
    entriesArray->isSorted = true;
    for (int i = 1; (i < entriesArray->firstNullEntry) ; i++) {
        if ((entriesArray->entry[i-1])->AEM < (entriesArray->entry[i])->AEM) {
            comps++;
            sortedElements++;
            continue;
        }
        //find the pos it should be in
        sortToPos = find(entriesArray, (entriesArray->entry[sortedElements])->AEM, sortedElements-1);
        comps += sortToPos.comps;
        tempPtr = entriesArray->entry[sortedElements];
        for (int i = sortedElements - sortToPos.guess; i > 0; i--) {
            // Shift sorted indexes after the position the number has to be in, by 1
            entriesArray->entry[sortToPos.guess+i] = entriesArray->entry[sortToPos.guess+i-1];
        }
        entriesArray->entry[sortToPos.guess] = tempPtr;
        sortedElements++;
    }
    return(comps);
}

// Pointer not needed but doesnt have to copy all info!
void print(databaseT* entriesArray) {
    printf("\n##\n");
    for (int i = 0; i < entriesArray->firstNullEntry; i++) {
        printf("%lu %s %s %hu\n", entriesArray->entry[i]->AEM, entriesArray->entry[i]->studentID.firstName,
                entriesArray->entry[i]->studentID.lastName, entriesArray->entry[i]->coursesFailed);
    }
}

void clearStdin() {
    char binInput;

    do {
        scanf("%c", &binInput);
    } while (binInput != '\n');
}

int main (int argc, char *argv[]) {
    databaseT entriesArray;
    studentEntryT studentDummyTemp;
    findIndexT foundTemp;
    int appendSize = 0, initialSize, courses;
    long unsigned int AEMtemp = 0;
    char input;
    char *firstNameTemp, *lastNameTemp;

    entriesArray.firstNullEntry = 0;

    if (argc != 3) {
        printf("Incorrect command line arguments!\n");
        exit(42);
    }
    else {
        appendSize = atoi(argv[1]);
        initialSize = atoi(argv[2]);
        entriesArray.entry = (studentEntryT**) malloc(sizeof(studentEntryT*)*initialSize);
        if (entriesArray.entry == NULL) {
            quit(&entriesArray);
        }
        entriesArray.size = initialSize;
        if (entriesArray.firstNullEntry >= entriesArray.size) {
            doRealloc(&entriesArray, appendSize);
        }
    }
 
    do {
        scanf(" %c", &input);
        switch (input) {
            case 'a': {
                // Append array if no more space
                if (entriesArray.firstNullEntry >= entriesArray.size) {
                    doRealloc(&entriesArray, appendSize);
                }
                //---   add
                // check if exists with find
                scanf("%lu", &studentDummyTemp.AEM);
                scanf(" %ms %ms", &firstNameTemp, &lastNameTemp);
                scanf(" %hu", &studentDummyTemp.coursesFailed);
                if (trimWord(&firstNameTemp, NAMESIZE) || trimWord(&lastNameTemp, NAMESIZE)) {
                    printf("Trimmed name.\n");
                }
                // clear word
                strcpy(studentDummyTemp.studentID.firstName, firstNameTemp);
                strcpy(studentDummyTemp.studentID.lastName, lastNameTemp);
                free(firstNameTemp);
                free(lastNameTemp);

                if (studentDummyTemp.AEM == 0 || studentDummyTemp.coursesFailed == 0) {
                    printf("Invalid input.\n");
                    clearStdin();
                }
                else if (add(&entriesArray, studentDummyTemp)) {
                    printf("A-OK %lu %d %d\n", studentDummyTemp.AEM, entriesArray.firstNullEntry, entriesArray.size);
                }
                else {
                    //if ((entriesArray.firstNullEntry)%appendSize == 0) {
                    //    doRealloc(&entriesArray, appendSize*(-1));
                    //}
                    printf("A-NOK %lu %d %hu\n", studentDummyTemp.AEM, entriesArray.firstNullEntry, entriesArray.size);
                    clearStdin();
                }
                break;
            }
            case 'r': {
                scanf("%lu", &AEMtemp);
                if (rmv(&entriesArray, AEMtemp, appendSize)) {
                    printf("R-OK %lu %d %hu\n", AEMtemp, entriesArray.firstNullEntry, entriesArray.size);
                } else {
                    printf("R-NOK %lu %d %hu\n", AEMtemp, entriesArray.firstNullEntry, entriesArray.size);
                }

                break;
            }
            case 'm': {
                scanf("%lu %d", &AEMtemp, &courses);
                if (mod(&entriesArray, AEMtemp, courses)) {
                    printf("M-OK %lu\n", AEMtemp);
                } else {
                    printf("M-NOK %lu\n", AEMtemp);
                }
                break;
            }
            case 'f':{
                scanf("%lu", &AEMtemp);
                foundTemp = find(&entriesArray, AEMtemp, 0);
                if(foundTemp.found == true) {
                    printf("F-OK %s %s %hu", entriesArray.entry[foundTemp.guess]->studentID.firstName, 
                                             entriesArray.entry[foundTemp.guess]->studentID.lastName,
                                             entriesArray.entry[foundTemp.guess]->coursesFailed);
                }
                else {
                    printf("F-NOK %lu", AEMtemp);
                }
                fprintf(stderr, "\n%lu\n", foundTemp.comps);
                break;
            }
            case 'p':{
                print(&entriesArray);
                break;
            }
            case 'c':{
                clear(&entriesArray);
                break;
            }
            case 'q':{
                quit(&entriesArray);
                break;
            }
            case 's':{
                printf("S-OK\n");
                fprintf(stderr, "\n%lu\n", sort(&entriesArray));
                break;
            }
            default:{
                clearStdin();
                printf("Invalid command.\n");

            }
        }
        
    } while (1); 
	return 0;
}

// a 45 DHSAGASD AHDGDASG 1
// a 8 ASDHG ASDHGASD 2
// a 56 SAHDGASD ASDHG 2
// a 1 ASDJHASJD HASDJHJA 2
// a 3467 ASDJH ASJDH 2
// a 96 ASDHGDAS ASDHG 2
// a 4 ASDHG ASDASDHG 2
// a 16 SADJH ASDJHASD 2
// a 7 SAHDFADF ASDGFASDGF 7

//Google: sorting alg hungarian dance 