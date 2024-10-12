/* ONOMATEPWNYMO: TsogkasPanagiotis Nikolaos
AEM: 03672
*/

/* 
    This programme stores student entries in a Database which can be modified (add, remove entries)
    Entries can also be modified, sorted, listed on screen
    Commands: p (print), c (clear db), a (add entry), r (remove entry), m (modify entry)
    q (exit programme), s (sort entries), f (find entry)

    Status: 0,1 (Normal exit), 42 (wrong arguments), 43 (Initial malloc fail)
*/

#include"project1.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>

typedef enum {false, true} boolean;

typedef struct {
    unsigned long int comps;
    boolean isFound;
    int index;
} findInfoT;

typedef struct {
    char firstName[NAMESIZE];
    char lastName[NAMESIZE];
} studentT;

typedef struct {
    unsigned short int coursesFailed;
    unsigned long int AEM;
    studentT studentID;
} studentEntryT;

typedef struct {
    studentEntryT **entry;
    int firstNullEntry;
    boolean isSorted;
    int initialSize;
    int appendSize;
    int size;
} databaseT;

void quit (databaseT *entriesArray, boolean normalExit);
void swap (databaseT *entriesArray, int firstIndex, int secondIndex);
void wordtoUpper (char* word);

// This function sorts a database's entries based on AEM values
// Ascending order
// Returns the number of comparisons that were needed for the database to 
// be sorted

unsigned long int sort (databaseT *entriesArray) {
    unsigned long int sortedElements, comps = 0;
    int i;

    for (sortedElements = 1; sortedElements < entriesArray->firstNullEntry; sortedElements++) {
        // Set i for loop
        i = sortedElements - 1;

        // Run untill it's in order
        for (; (i >= 0); i--) {
            if (entriesArray->entry[i+1]->AEM < entriesArray->entry[i]->AEM) {
                swap(entriesArray, i, i+1);
            }
            else {
                comps++;
                break;
            }
            comps++;
        }
    }

    entriesArray->isSorted = true;
    return(comps);
}

// Binary search function
// Returns stuct findInfoT which contains the variables isFound, index, comps (comparisons)

findInfoT bin_search (studentEntryT **array, unsigned long int targetAEM, int lowerBound, int upperBound, findInfoT result) {    
    
    result.index = lowerBound + (upperBound - lowerBound)/2;

    // Nothing to check hasnt found the AEM
    if (result.index > upperBound) {
        result.isFound = false;
        return(result);
    }

    // Check if current is the key
    if (array[result.index]->AEM == targetAEM) {
        result.isFound = true;
        result.comps++;
        return(result);
    }

    // Checked last not found.
    if (result.index == upperBound) {
        result.isFound = false;
        return(result);
    }
    // Maybe to the left?
    else if (array[result.index]->AEM > targetAEM) {
        result.comps += 2;
        result = bin_search(array, targetAEM, lowerBound, result.index-1, result);
    }
    // Maybe to the right?
    else {
        result.comps += 2;
        result = bin_search(array, targetAEM, result.index+1, upperBound, result);
    }

    // How did we get here?
    return(result);
}

// Find function: Finds an entry with a specified AEM value
// it has 2 search types (Binary, linear)
// Returns stuct findInfoT which contains the variables isFound, index, comps (comparisons)

findInfoT findStudent (databaseT *entriesArray, unsigned long int targetAEM) {
    findInfoT result;
    int lowerBound, upperBound;

    // Initialise variables
    upperBound = entriesArray->firstNullEntry-1;
    result.isFound = false;
    lowerBound = 0;

    // Run linear search
    if (entriesArray->isSorted == false) {
        for (result.index = 0, result.comps = 0; result.index < entriesArray->firstNullEntry; result.index++) {
            result.comps++;
            if (targetAEM == entriesArray->entry[result.index]->AEM) {
                result.isFound = true;
                break;  
            }
        }
        return(result);
    }

    // Run Binary search
    result.comps = 0;
    return(bin_search(entriesArray->entry, targetAEM, lowerBound, upperBound, result));
}

// This is a function that changes the size of a database (if needed) to fit more entries
// Returns true if the size of the database was altered and false if not

boolean doRealloc (databaseT *entriesArray, boolean enlarge) {
    studentEntryT **studArrayTemp;

    if (enlarge == true) {
        // Make array bigger
        if (entriesArray->size <= entriesArray->firstNullEntry) {
            if (entriesArray->size + entriesArray->appendSize != 0) {
                studArrayTemp = (studentEntryT**) realloc(entriesArray->entry, (entriesArray->size + entriesArray->appendSize) * sizeof(studentEntryT*));
                if (studArrayTemp == NULL) return(false);
                else {
                    entriesArray->entry = studArrayTemp;
                    // Increase array size
                    entriesArray->size += entriesArray->appendSize;
                }
            }
        }
    }
    else {
        // Make array smaller
        if (entriesArray->size - entriesArray->appendSize <= 0) return(false);
        // Only if enough students have been rmved, do realloc
        else if (entriesArray->size - entriesArray->firstNullEntry >= entriesArray->appendSize) {            
            studArrayTemp = (studentEntryT**) realloc(entriesArray->entry, (entriesArray->size - entriesArray->appendSize) * sizeof(studentEntryT*));
            if (studArrayTemp == NULL) return(false);
            else {
                entriesArray->entry = studArrayTemp;
                // Reduce array size
                entriesArray->size -= entriesArray->appendSize;
            }
        }
    }
    // Fail
    return(false);
}

// This function adds an entry to a database increasing its size if need be
// Returns true if addition was successful, false if it wasnt

boolean addStudent (databaseT *entriesArray, studentEntryT studentInfo) {

    // If false then add a student
    if (findStudent(entriesArray, studentInfo.AEM).isFound == false) {
        // If no space realloc will be executed
        doRealloc(entriesArray, true);
        
        // Check if Realloc failed when size was 0
        if ((entriesArray->entry != NULL)) {
            // Create memory
            entriesArray->entry[entriesArray->firstNullEntry] = (studentEntryT*) malloc(sizeof(studentEntryT));
            if (entriesArray->entry[entriesArray->firstNullEntry] != NULL) {
                *(entriesArray->entry[entriesArray->firstNullEntry]) = studentInfo;
                // increase 1st null entry by 1 
                entriesArray->firstNullEntry++;

                // Unless he goes last
                entriesArray->isSorted = false;
                return(true);
            }
        }
    }
    // else student already exists
    return(false);
}

// This function removes an entry from a database with a specified AEM value decerasing it's size if needed
// Returns true if removal was successful and false if it wasnt 

boolean rmvStudent (databaseT *entriesArray, unsigned long int targetAEM) {
    findInfoT searchRes;
    int lastStudent = entriesArray->firstNullEntry - 1;

    searchRes = findStudent(entriesArray, targetAEM);
    
    // Check if student exists so as to remove him
    if (searchRes.isFound == true) {
        // Free memory for rmved student 
        free(entriesArray->entry[searchRes.index]);

        // If it's the last student
        if (searchRes.index == lastStudent) {
            entriesArray->entry[lastStudent] = NULL;
            entriesArray->firstNullEntry--;
            // Decrease size of array if enough free space
            doRealloc(entriesArray, false);
            return(true);
        }

        // Swap last with cur
        entriesArray->entry[searchRes.index] = entriesArray->entry[lastStudent];
        entriesArray->entry[lastStudent] = NULL;
        // Decrease 1st null entry by 1
        entriesArray->firstNullEntry--;
        // Decrease size of array if enough free space
        doRealloc(entriesArray, false);

        entriesArray->isSorted = false;
        // Removed
        return(true);
    }

    // Couldnt remove
    return(false);
}

// This function trims a word down to a specified size and turns all letters to upper case
// while also freeing up memory that isnt used
// Returns true if word was trimmed, false if it wasnt 

boolean trimWord (char **word, int size) {
    int i;

    for (i = 0; (*word)[i] != '\0'; i++);
    if (i > size) {
        *word = (char*) realloc(*word, size*sizeof(char));
        (*word)[size-1] = '\0';
        wordtoUpper(*word);
        return(true);
    }
    wordtoUpper(*word);
    return(false);
}

// This function mods a specified student's failed courses
// Returns true if successful, false if it failed

boolean modStudent (databaseT *entriesArray, int coursesFailed, unsigned long int targetAEM) {
    findInfoT searchRes;

    searchRes = findStudent(entriesArray, targetAEM);

    if (searchRes.isFound == true) {
        entriesArray->entry[searchRes.index]->coursesFailed = coursesFailed;
        // Success
        return(true);
    }
    // Failed
    return(false);
}

// Turns a word to upper case only
void wordtoUpper (char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = toupper(word[i]);
    }
}

// This function hundles the programme's arguments

void handleArguments (databaseT *entriesArray, int argc, char *argv[]) {
    if (argc != 3) {
        // Not enough / Too many arguments
        exit(42);
    }
    else {
        // Set initial parameters
        entriesArray->appendSize = atoi(argv[2]);
        entriesArray->initialSize = atoi(argv[1]);
    }
}

// This function does malloc and handles malloc failure

void doMalloc (databaseT *entriesArray) {
    studentEntryT **studArrayTemp;

    studArrayTemp = (studentEntryT**) malloc(entriesArray->initialSize * sizeof(studentEntryT*));
    studArrayTemp == NULL ? quit(entriesArray, false) : (entriesArray->entry = studArrayTemp);
    entriesArray->size = entriesArray->initialSize;
}

// Initialises a database

void init_db (databaseT *entriesArray, int argc, char* argv[]) {
    entriesArray->firstNullEntry = 0;
    entriesArray->isSorted = false;
    entriesArray->entry = NULL;

    // Set initial parameters for db if possible
    handleArguments(entriesArray, argc, argv);
    doMalloc(entriesArray);
}

// swaps 2 entries of a database

void swap (databaseT *entriesArray, int firstIndex, int secondIndex) {
    studentEntryT *temp;

    temp = entriesArray->entry[firstIndex];
    entriesArray->entry[firstIndex] = entriesArray->entry[secondIndex];
    entriesArray->entry[secondIndex] = temp;
}

// This function prints all student entries in database

// Pointer not needed but doesnt have to copy all info!
void print(databaseT* entriesArray) {
    printf("\n##\n");
    for (int i = 0; i < entriesArray->firstNullEntry; i++) {
        printf("%lu %s %s %hu\n", entriesArray->entry[i]->AEM, entriesArray->entry[i]->studentID.firstName,
                entriesArray->entry[i]->studentID.lastName, entriesArray->entry[i]->coursesFailed);
    }
}

// This function clears the database and frees all memory 

void clear (databaseT *entriesArray) {
    for (int i = 0; i < entriesArray->firstNullEntry; i++) {
        free(entriesArray->entry[i]);
    }
    entriesArray->size = 0;
    entriesArray->firstNullEntry = 0;
    if (entriesArray->entry != NULL) {
        free(entriesArray->entry);
        entriesArray->entry = NULL;
    }
}

// This function clears the database and exits the programme

void quit (databaseT *entriesArray, boolean normalExit) {
    clear(entriesArray);
    if (normalExit) {
        exit(0);
    }
    exit(43);
}

// Clears all input  until \n is reached

void clearStdin() {
    char binInput;

    do {
        scanf("%c", &binInput);
    } while (binInput != '\n');
}


int main (int argc, char *argv[]) {
    studentEntryT studentInfo;
    boolean isNameTrimmed, isLastNameTrimmed;
    char *firstNameTemp, *lastNameTemp;
    databaseT entriesArray;
    findInfoT searchRes;
    char cmdInput;
    

    // Initialise Database
    init_db(&entriesArray, argc, argv);

    // Handle commands (stdin)
    do {
        scanf(" %c", &cmdInput);
        switch (cmdInput) {
            // Add a student entry
            case 'a': {
                // Scan info
                scanf("%lu", &studentInfo.AEM);
                scanf(" %ms %ms", &firstNameTemp, &lastNameTemp);
                scanf(" %hu", &studentInfo.coursesFailed);
                //Trim names

                isNameTrimmed = trimWord(&firstNameTemp, NAMESIZE);
                isLastNameTrimmed = trimWord(&lastNameTemp, NAMESIZE);
                if (isLastNameTrimmed || isNameTrimmed) {
                    printf("\nTrimmed name(s).\n");
                }
                strcpy(studentInfo.studentID.firstName, firstNameTemp);
                strcpy(studentInfo.studentID.lastName, lastNameTemp);
                free(firstNameTemp);
                free(lastNameTemp);

                // If valid info
                if ((studentInfo.AEM != 0) && (studentInfo.coursesFailed != 0)) {
                    // Add a new student
                    if (addStudent(&entriesArray, studentInfo)) {
                        // Successful
                        printf("\nA-OK %lu, %d %d\n", studentInfo.AEM, entriesArray.firstNullEntry, entriesArray.size);
                    }
                    else {
                        // Failed
                        printf("\nA-NOK %lu, %d %hu\n", studentInfo.AEM, entriesArray.firstNullEntry, entriesArray.size);
                    }
                }
                else {
                    printf("\nInvalid input.\n");
                }

                // Clear untli \n
                clearStdin();
                break;
            }
            // Remove a student enrty
            case 'r': {
                scanf("%lu", &studentInfo.AEM);
                if (rmvStudent(&entriesArray, studentInfo.AEM)) {
                    // Successful  
                    printf("\nR-OK %lu, %d %hu\n", studentInfo.AEM, entriesArray.firstNullEntry, entriesArray.size);
                } else {
                    // Failed
                    printf("\nR-NOK %lu, %d %hu\n", studentInfo.AEM, entriesArray.firstNullEntry, entriesArray.size);
                }

                // Clear untli \n
                clearStdin();
                break;
            }
            // Modify a student entry
            case 'm': {
                scanf(" %lu %hu", &studentInfo.AEM, &studentInfo.coursesFailed);
                if ((studentInfo.AEM != 0) && (studentInfo.coursesFailed != 0)) {
                    if (modStudent(&entriesArray, studentInfo.coursesFailed, studentInfo.AEM)) {  
                        // Successful  
                        printf("\nM-OK %lu\n", studentInfo.AEM);
                    } else {
                        // Failed
                        printf("\nM-NOK %lu\n", studentInfo.AEM);
                    }
                }
                else {
                    printf("\nInvalid input.\n");
                }
                // Clear untli \n
                clearStdin();
                break;
            }
            // Find a student entry
            case 'f': {
                scanf("%lu", &studentInfo.AEM);
                searchRes =  findStudent(&entriesArray, studentInfo.AEM);
                if (searchRes.isFound) {
                    // Successful
                    printf("\nF-OK %s %s %hu\n", entriesArray.entry[searchRes.index]->studentID.firstName, 
                                             entriesArray.entry[searchRes.index]->studentID.lastName,
                                             entriesArray.entry[searchRes.index]->coursesFailed);
                }
                else {
                    // Failed
                    printf("\nF-NOK %lu\n", studentInfo.AEM);
                }
                // Print comparisons between entries
                fprintf(stderr, "\n$%lu\n", searchRes.comps);
                // Clear untli \n
                clearStdin();
                break;
            }
            // Sort database
            case 's': {
                printf("\nS-OK\n");
                // Print comparisons between entries
                fprintf(stderr, "\n$%lu\n", sort(&entriesArray));
                break;
            }
            // Print all entries
            case 'p': {
                print(&entriesArray);
                break;
            }
            // Clear database
            case 'c': {
                clear(&entriesArray);
                printf("\nC-OK\n");
                break;
            }
            // Exit programme
            case 'q': {
                quit(&entriesArray, true);
                break;
            }
        }
    } while(1);

	return 0;
}
