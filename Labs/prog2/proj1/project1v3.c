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

int idk = 0;

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

        //*tempWord = (char*) realloc(*word, (trimSize)*sizeof(char));
        if (tempWord != NULL) {
            //word = tempWord;
            (*word)[trimSize-1] = '\0';
            wordtoUpper(*word);
            return(true);
        } else {
            wordtoUpper(*word);
            return(false);
        }
    }
    
    wordtoUpper(*word);
    return(false);
}

findIndexT find (databaseT *entriesArray, long unsigned int targetAEM) {
    int current_index, move_factor = 0;
    findIndexT valueReturn;

    valueReturn.comps = 0;

    if ((entriesArray->firstNullEntry)-1 < 0) {
        valueReturn.found = false;
        return(valueReturn);
    }
    current_index = (entriesArray->firstNullEntry)-1;
    move_factor = current_index;

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
            valueReturn.comps += 2; 
            valueReturn.found = false;
            valueReturn.guess = 0;
            return(valueReturn);
        // if last index is target
        // found
        } else if ((entriesArray->entry[current_index])->AEM == targetAEM) {
            valueReturn.comps+=3; 
            valueReturn.found = true;
            valueReturn.guess = current_index;
            return(valueReturn);
        // if > last index it should remain in it's pos
        // not found so guess
        } else if ((entriesArray->entry[current_index])->AEM < targetAEM) {
            valueReturn.comps+=4; 
            valueReturn.found = false;
            valueReturn.guess = current_index+1;
            return(valueReturn);
        }
        valueReturn.comps+=4;
        do {
            // move by move_factor
            move_factor /= 2;
            // move left
            if (((entriesArray->entry[current_index])->AEM) > targetAEM) {
                valueReturn.comps++; 
                current_index -= move_factor;
            }
            // move right
            else if (((entriesArray->entry[current_index])->AEM) < targetAEM) {
                valueReturn.comps+=2; 
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
            if (move_factor == 0) {
                valueReturn.found = false;
                valueReturn.guess = current_index;
                return(valueReturn);
            }
        } while (1);
    }
    else {
        // Linear serach
        for (int i = 0; (i < entriesArray->firstNullEntry) && (entriesArray->entry[i] != NULL); i++) {
            valueReturn.comps++; 
            if ((entriesArray->entry[i])->AEM == targetAEM) {
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
    for (int i = 0; i < entriesArray->firstNullEntry; i++) {
        free(entriesArray->entry[i]);
    }
    entriesArray->size = 0;
    entriesArray->firstNullEntry = 0;
}

// Exit programme
void quit (databaseT *entriesArray) {
    clear(entriesArray);
    exit(0);
}

// A function that does all the work for me :)
// (Check if realloc fails + set new value if the pointer changes)
boolean doRealloc (databaseT *entriesArray, int appendSize) {
    studentEntryT** tempPtr;
    
    if ((entriesArray->size + appendSize) < 0) return(false);

    tempPtr = entriesArray->entry;
    entriesArray->entry = (studentEntryT**) realloc(entriesArray->entry, (entriesArray->size + appendSize)*sizeof(studentEntryT*));
    if (entriesArray->entry == NULL) {
        entriesArray->entry = tempPtr;
        return(false);
    }
    return(true);
}

// Checks if malloc has failed. 
void mallocFail (databaseT *entriesArray, int indexToCheck) {
    if (entriesArray->entry[indexToCheck] == NULL) {
        clear(entriesArray);
        exit(43);
    }
}

// Add memory for a student entry. returns true is success false if fails
boolean add (databaseT *entriesArray, studentEntryT studentDummyTemp, int appendSize) {
    studentEntryT* temp;

    if ((find(entriesArray, studentDummyTemp.AEM)).found == false) {
        temp = (studentEntryT*) malloc(sizeof(studentEntryT));
        if (temp == NULL) {
            return(false);
        }
        if (entriesArray->size <= entriesArray->firstNullEntry) {
            if(doRealloc(entriesArray, appendSize) == false) {
                free(temp);
                return(false);
            }
            entriesArray->size += appendSize;
        }
        entriesArray->entry[entriesArray->firstNullEntry] = temp;
        temp = NULL;
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
    findIndexT temp;
    
    temp = find(entriesArray, foundAEM);
    if ((temp.found == true) && (entriesArray->firstNullEntry-1 == temp.guess)) {
        free(entriesArray->entry[temp.guess]);
        entriesArray->entry[temp.guess] = NULL;
        (entriesArray->firstNullEntry)--;
        if (((entriesArray->size - entriesArray->firstNullEntry)%appendSize >= 0) && ((entriesArray->size - entriesArray->firstNullEntry) >= appendSize)) {
            if(doRealloc(entriesArray, appendSize*(-1)) == true) {
                (entriesArray->size)-=appendSize;
            }
        }
        return(true);
    }
    if (temp.found == true) {
        free(entriesArray->entry[temp.guess]);
        entriesArray->entry[temp.guess] = entriesArray->entry[(entriesArray->firstNullEntry)-1];
        entriesArray->entry[(entriesArray->firstNullEntry)-1] = NULL;
        (entriesArray->firstNullEntry)--;
        if (((entriesArray->size - entriesArray->firstNullEntry)%appendSize >= 0) && ((entriesArray->size - entriesArray->firstNullEntry) >= appendSize)) {
            if(doRealloc(entriesArray, appendSize*(-1)) == true) {
                (entriesArray->size)-=appendSize;
            }
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
    
    indexFoundIn = find(entriesArray, targetAEM);
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
    studentEntryT *tempPtr;

    //sortedElements = first unsorted element
    for (int sorted = 1; sorted < entriesArray->firstNullEntry; sorted++) {
        if (entriesArray->entry[sorted-1]->AEM < entriesArray->entry[sorted]->AEM) {
            comps++;
            continue;
        }
        for (int i = sorted; (i >= 1); i--) {
            comps++;
            if (entriesArray->entry[i-1]->AEM < entriesArray->entry[i]->AEM) {
                break;
            }
            tempPtr = entriesArray->entry[i];
            entriesArray->entry[i] = entriesArray->entry[i-1];
            entriesArray->entry[i-1] = tempPtr;
        }
    }
    entriesArray->isSorted = true;
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
    boolean isfrstNameTrimmed, islastNameTrimmed;

    entriesArray.entry = NULL;
    entriesArray.firstNullEntry = 0;

    if (argc != 3) {
        printf("Incorrect command line arguments!\n");
        exit(42);
    }
    else {
        appendSize = atoi(argv[2]);
        initialSize = atoi(argv[1]);
        entriesArray.entry = (studentEntryT**) malloc(sizeof(studentEntryT*)*initialSize);
        if (entriesArray.entry == NULL) {
            quit(&entriesArray);
        }
        entriesArray.size = initialSize;
    }
 
    do {
        scanf(" %c", &input);
        switch (input) {
            case 'a': {
                //---   add
                // check if exists with find
                scanf("%lu", &studentDummyTemp.AEM);
                scanf(" %ms %ms", &firstNameTemp, &lastNameTemp);
                scanf(" %hu", &studentDummyTemp.coursesFailed);
                islastNameTrimmed = trimWord(&lastNameTemp, NAMESIZE);
                isfrstNameTrimmed = trimWord(&firstNameTemp, NAMESIZE);
                if (islastNameTrimmed || isfrstNameTrimmed) {
                    printf("\nTrimmed name(s).\n");
                }
                // clear word
                strcpy(studentDummyTemp.studentID.firstName, firstNameTemp);
                strcpy(studentDummyTemp.studentID.lastName, lastNameTemp);
                free(lastNameTemp);
                free(firstNameTemp);

                if (studentDummyTemp.AEM == 0 || studentDummyTemp.coursesFailed == 0) {
                    printf("\nInvalid input.\n");
                    clearStdin();
                }
                else if (add(&entriesArray, studentDummyTemp, appendSize)) {
                    printf("\nA-OK %lu, %d %d\n", studentDummyTemp.AEM, entriesArray.firstNullEntry, entriesArray.size);
                }
                else {
                    printf("\nA-NOK %lu, %d %hu\n", studentDummyTemp.AEM, entriesArray.firstNullEntry, entriesArray.size);
                    clearStdin();
                }
                break;
            }
            case 'r': {
                scanf("%lu", &AEMtemp);
                if (rmv(&entriesArray, AEMtemp, appendSize)) {
                    printf("\nR-OK %lu, %d %hu\n", AEMtemp, entriesArray.firstNullEntry, entriesArray.size);
                } else {
                    printf("\nR-NOK %lu, %d %hu\n", AEMtemp, entriesArray.firstNullEntry, entriesArray.size);
                }

                break;
            }
            case 'm': {
                scanf("%lu %d", &AEMtemp, &courses);
                if (courses == 0) {
                    printf("\nInvalid input.\n");
                }
                else {
                    if (mod(&entriesArray, AEMtemp, courses)) {
                        printf("\nM-OK %lu\n", AEMtemp);
                    } else {
                        printf("\nM-NOK %lu\n", AEMtemp);
                    }
                }
                break;
            }
            case 'f':{
                scanf("%lu", &AEMtemp);
                foundTemp = find(&entriesArray, AEMtemp);
                if(foundTemp.found == true) {
                    printf("\nF-OK %s %s %hu\n", entriesArray.entry[foundTemp.guess]->studentID.firstName, 
                                             entriesArray.entry[foundTemp.guess]->studentID.lastName,
                                             entriesArray.entry[foundTemp.guess]->coursesFailed);
                }
                else {
                    printf("\nF-NOK %lu\n", AEMtemp);
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
                printf("\nC-OK\n");
                break;
            }
            case 'q':{
                quit(&entriesArray);
                break;
            }
            case 's':{
                printf("\nS-OK\n");
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