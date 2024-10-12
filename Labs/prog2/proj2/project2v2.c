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
#include"project2.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef enum {DOWN = -1, UP = 1} handleSize;

typedef struct listStr {
    struct listStr *next;
    int courseId;
} list;

typedef struct {
    int student;
    list *prev;
    bool found;
    bool empty;
} listFoundT;

typedef struct {
    unsigned long int comps;
    bool isFound;
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
    list *courses;
} studentEntryT;

typedef struct linkedList {
    struct linkedList *next;
    struct linkedList *prev;
    studentEntryT *studentPtr;
} linked_list;

typedef struct {
    studentEntryT **entry;
    int numOfEntries;
    bool isSorted;
    int initialSize;
    int appendSize;
    int size;
} databaseT;

typedef struct {
    linked_list *sentinel;
    int list_size;
} bucketT;

typedef struct {
    bucketT *lnk_list;
    int initialSize;
    int elements;
    int size;
} hashT;

void quit (databaseT *entriesArray, bool normalExit);
void swap (databaseT *entriesArray, int firstIndex, int secondIndex);
void wordtoUpper (char* word);

// This function sorts a database's entries based on AEM values
// Ascending order
// Returns the number of comparisons that were needed for the database to 
// be sorted

unsigned long int sort (databaseT *entriesArray) {
    unsigned long int sortedElements, comps = 0;
    int i;

    for (sortedElements = 1; sortedElements < entriesArray->numOfEntries; sortedElements++) {
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


//

unsigned long int hash(char *str) {
    unsigned long int hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
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
    upperBound = entriesArray->numOfEntries-1;
    result.isFound = false;
    lowerBound = 0;

    // Run linear search
    if (entriesArray->isSorted == false) {
        for (result.index = 0, result.comps = 0; result.index < entriesArray->numOfEntries; result.index++) {
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

//

bool listMalloc (list **head) {
    list *temp;

    temp = (list*) malloc(sizeof(list));
    if (temp == NULL) return(false);
    *head = temp;

    return(true);
}

bool linked_listMalloc (linked_list **head) {
    linked_list *temp;

    temp = (linked_list*) malloc(sizeof(linked_list));
    if (temp == NULL) return(false);
    *head = temp;

    return(true);
}

//

listFoundT isreg (databaseT *entriesArray, unsigned long int targetAEM, int courseId) {
    listFoundT result_isreg;
    findInfoT result_find;
    list *curr = NULL;

    // Initialise result
    result_isreg.empty = false;
    result_isreg.found = false;
    result_isreg.prev = NULL;

    result_find = findStudent(entriesArray, targetAEM);
    result_isreg.student = result_find.index;

    // No student found
    if (result_find.isFound == false) {
        result_isreg.student = -1;    
        return(result_isreg);
    }
    // Student found but has no courses
    if (entriesArray->entry[result_find.index]->courses == NULL) {
        result_isreg.empty = true;
        return(result_isreg);
    }
    // Run list
    for (curr = entriesArray->entry[result_find.index]->courses; (curr!=NULL) && (curr->courseId < courseId); curr = curr->next) {
        result_isreg.prev = curr;
    }
    // If found set found to true true
    if ((curr != NULL) && (curr->courseId == courseId)) result_isreg.found = true;
    
    // return result
    return(result_isreg);
}


//

int reg (databaseT *entriesArray, unsigned long int targetAEM, int courseId) {
    listFoundT searchRes;
    list *tempNode, *currentNode;

    searchRes = isreg(entriesArray, targetAEM, courseId);
    
    // already exists
    if (searchRes.found) {
        return(1);
    }
    else if (searchRes.student == -1) {
        return(2);
    }
    else if (searchRes.prev == NULL) {
        if (listMalloc(&(entriesArray->entry[searchRes.student]->courses))) {
            entriesArray->entry[searchRes.student]->courses->courseId = courseId;
            entriesArray->entry[searchRes.student]->courses->next = searchRes.prev;
        }
        else return(3);
    }
    else if (listMalloc(&(tempNode))) {
        currentNode = (searchRes.prev)->next;
        tempNode->courseId = courseId;
        (searchRes.prev)->next = tempNode;
        tempNode->next = currentNode;
    }
    else return(3);


    return(0);
}


// 

int unreg (databaseT *entriesArray, unsigned long int targetAEM, int courseId) {
    listFoundT searchRes;
    list *tempNode;

    searchRes = isreg(entriesArray, targetAEM, courseId);
    if (searchRes.found == false || searchRes.empty == true) return(1);
    if (searchRes.student == -1) return(2);
    if (searchRes.prev == NULL) {
        tempNode = entriesArray->entry[searchRes.student]->courses;
        entriesArray->entry[searchRes.student]->courses = entriesArray->entry[searchRes.student]->courses->next;
        free(tempNode);
    }
    else {
        // Notice that prev can never be the last index so no segm fault
        tempNode = (searchRes.prev)->next;
        (searchRes.prev)->next = (searchRes.prev)->next->next; 
        free(tempNode);
    }
    return(0);
}


//

bool list_courses (databaseT *entriesArray, unsigned long int targetAEM) {
    findInfoT searchRes;

    searchRes = findStudent(entriesArray, targetAEM);

    if (searchRes.isFound == false) return(false);

    printf("\nL-OK %s %lu\n", entriesArray->entry[searchRes.index]->studentID.lastName, targetAEM);
    for (list *curr = entriesArray->entry[searchRes.index]->courses; curr != NULL; curr = curr->next) {
        printf("%d\n", curr->courseId);
    }

    return(true);
}

// This is a function that changes the size of a database (if needed) to fit more entries
// Returns true if the size of the database was altered and false if not

bool doRealloc (databaseT *entriesArray, handleSize scaler) {
    studentEntryT **studArrayTemp;

    // No need to change size already big enough
    if ((scaler == 1) && (entriesArray->size > entriesArray->numOfEntries)) return(false);
    // Cant decrease size of 0
    if ((scaler == -1) && (entriesArray->size - entriesArray->appendSize <= 0)) return(false);
    // Not enough free entries to decrease size
    if ((scaler == -1) && (entriesArray->size - entriesArray->numOfEntries >= entriesArray->appendSize)) return(false);

    studArrayTemp = (studentEntryT**) realloc(entriesArray->entry, (entriesArray->size + scaler * entriesArray->appendSize) * sizeof(studentEntryT*));
    if (studArrayTemp == NULL) return(false);
    else {
        entriesArray->entry = studArrayTemp;
        // Change array size
        entriesArray->size = entriesArray->size + scaler * entriesArray->appendSize;
    }
    
    // Fail
    return(false);
}

// This function adds an entry to a database increasing its size if need be
// Returns true if addition was successful, false if it wasnt

bool addStudent (databaseT *entriesArray, studentEntryT studentInfo) {

    // If false then add a student
    if (findStudent(entriesArray, studentInfo.AEM).isFound == false) {
        // If no space realloc will be executed
        doRealloc(entriesArray, UP);
        
        // Check if Realloc failed when size was 0
        if ((entriesArray->entry != NULL)) {
            // Create memory
            entriesArray->entry[entriesArray->numOfEntries] = (studentEntryT*) malloc(sizeof(studentEntryT));
            if (entriesArray->entry[entriesArray->numOfEntries] != NULL) {
                *(entriesArray->entry[entriesArray->numOfEntries]) = studentInfo;
                // increase 1st null entry by 1 
                entriesArray->numOfEntries++;

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

bool rmvStudent (databaseT *entriesArray, unsigned long int targetAEM) {
    findInfoT searchRes;
    int lastStudent = entriesArray->numOfEntries - 1;

    searchRes = findStudent(entriesArray, targetAEM);
    
    // Check if student exists so as to remove him
    if (searchRes.isFound == true) {
        // Free memory for rmved student 
        free(entriesArray->entry[searchRes.index]);

        // If it's the last student
        if (searchRes.index == lastStudent) {
            entriesArray->entry[lastStudent] = NULL;
            entriesArray->numOfEntries--;
            // Decrease size of array if enough free space
            doRealloc(entriesArray, DOWN);
            return(true);
        }

        // Swap last with cur
        entriesArray->entry[searchRes.index] = entriesArray->entry[lastStudent];
        entriesArray->entry[lastStudent] = NULL;
        // Decrease 1st null entry by 1
        entriesArray->numOfEntries--;
        // Decrease size of array if enough free space
        doRealloc(entriesArray, DOWN);

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

bool trimWord (char **word, int size) {
    int i;

    for (i = 0; (*word)[i] != '\0'; i++);
    if (i >= size) {
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

bool modStudent (databaseT *entriesArray, int coursesFailed, unsigned long int targetAEM) {
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
    entriesArray->numOfEntries = 0;
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
    for (int i = 0; i < entriesArray->numOfEntries; i++) {
        printf("%lu %s %s %hu\n", entriesArray->entry[i]->AEM, entriesArray->entry[i]->studentID.firstName,
                entriesArray->entry[i]->studentID.lastName, entriesArray->entry[i]->coursesFailed);
    }
}

// This function clears the database and frees all memory 

void clear (databaseT *entriesArray) {
    for (int i = 0; i < entriesArray->numOfEntries; i++) {
        free(entriesArray->entry[i]);
    }
    entriesArray->size = 0;
    entriesArray->numOfEntries = 0;
    if (entriesArray->entry != NULL) {
        free(entriesArray->entry);
        entriesArray->entry = NULL;
    }
}

// This function clears the database and exits the programme

void quit (databaseT *entriesArray, bool normalExit) {
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


void init_lnkList (linked_list *sentinel) {
    sentinel->studentPtr = NULL;
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
}

int compareWords (char *word, char *word_to_cmpr) {
    for (int i = 0; (word[i] != '\0') && (word_to_cmpr[i] != '\0'); i++) {
        if (word[i] < word_to_cmpr[i]) return(1);
        else if (word[i] > word_to_cmpr[i]) return(2);
        else continue;
    }
    return(0);
}

linked_list *find_lnkList (linked_list *sentinel, studentEntryT *student) {
    linked_list *curr;

    sentinel->studentPtr = student;

    for (curr = sentinel->next; compareWords(curr->studentPtr->studentID.lastName, student->studentID.lastName) == 1; curr = curr->next);
    while ((compareWords(curr->studentPtr->studentID.lastName, student->studentID.lastName) == 0) && (curr->studentPtr->AEM > student->AEM)) {
        curr = curr->next;
    }


    sentinel->studentPtr = NULL;

    return(curr);
}

bool add_lnkList (linked_list *sentinel, studentEntryT *student) {
    linked_list *result, *temp;
    

    result = find_lnkList(sentinel, student);
    if (result->studentPtr->studentID.lastName == student->studentID.lastName) {
        // Found
        return(false);
    }
    else if (linked_listMalloc(&temp)) {
        temp->studentPtr = student;
        temp->prev = result->prev;
        result->prev->next = temp;
        temp->next = result;
        result->prev = temp;

        return(true);
    }

    return(false);
}

void init_hash (hashT *hashTable, int initialSize) {
    hashTable->elements = 0;
    hashTable->initialSize = initialSize;
    hashTable->size = initialSize;
    hashTable->lnk_list->list_size = 0;
    for (int i = 0; i < hashTable->size; i++) {
        init_lnkList(hashTable->lnk_list[i].sentinel);
    }
}

hashT *createHash (int size) {
    hashT *temp;

    temp = (hashT*) malloc(sizeof(hashT));
    if (temp == NULL) return(NULL);
    temp->lnk_list = (bucketT*) malloc(size * sizeof(bucketT));
    if (temp->lnk_list == NULL) return(NULL);

    for (int i = 0; i < size; i++) {
        temp->lnk_list[i].sentinel = (linked_list*) malloc(sizeof(linked_list));
        if (temp->lnk_list[i].sentinel == NULL) return(false);
    }

    init_hash(temp, size);
    
    return(temp);
}

bool add_to_hash (hashT *hashTable, studentEntryT *student) {
    linked_list *result;
    int index;

    index = hash(student->studentID.lastName)%hashTable->size;
    result = find_lnkList(hashTable->lnk_list[index].sentinel, student);
    if (result->studentPtr->studentID.lastName == student->studentID.lastName) {
        // Found
        return(false);
    }
    else {
        if (add_lnkList(hashTable->lnk_list[index].sentinel, student)) {
            hashTable->elements++;
            hashTable->lnk_list[index].list_size++;
        }
    }
    
    return(true);
}

void deleteList (linked_list *list) {
    linked_list *curr = list->prev;
    
    if (curr != list) {
        do {
            curr = curr->prev;
            free(curr->next);
        } while (curr != list);
    }
    free(list);
}

void deleteHash (hashT *hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        deleteList(hashTable->lnk_list[i].sentinel);
    }
    free(hashTable->lnk_list);
    free(hashTable);
}

// Append does not word FIX THIS SHIT

bool doReHash (hashT **hashTable) {
    hashT *newTable;

    if (((double)(*hashTable)->elements / (double)(*hashTable)->size <= LOW_LOAD) && ((*hashTable)->size / 2 > (*hashTable)->initialSize)) {
        newTable = createHash((*hashTable)->size/2);
        newTable->initialSize = (*hashTable)->initialSize;
    }
    else if (((double)(*hashTable)->elements / (double)(*hashTable)->size > HIGH_LOAD)) {
        newTable = createHash((*hashTable)->size*2);
        newTable->initialSize = (*hashTable)->initialSize;
    }
    else {
        return(false);
    }

    if(newTable == NULL) return(false);
    for (int i = 0; i < (*hashTable)->size; i++) {
        for (linked_list *curr = (*hashTable)->lnk_list[i].sentinel->next; curr != (*hashTable)->lnk_list[i].sentinel; curr = curr->next) {
            add_to_hash(newTable, curr->studentPtr);
        } 
    }
    newTable->initialSize = (*hashTable)->initialSize;
    deleteHash((*hashTable));
    (*hashTable) = newTable;
    
    return(true);
}

bool rmv_from_hash (hashT *hashTable, studentEntryT *student) {
    linked_list *result, *temp;
    int index;

    index = hash(student->studentID.lastName)%hashTable->size;
    result = find_lnkList(hashTable->lnk_list[index].sentinel, student);

    if (result->studentPtr == student) {
        // Found
        temp = result;
        result->prev->next = temp->next;
        free(temp);
        return(true);
    }

    return(false);
}


int main (int argc, char *argv[]) {
    studentEntryT studentInfo;
    bool isNameTrimmed, isLastNameTrimmed;
    char *firstNameTemp, *lastNameTemp;
    databaseT entriesArray;
    findInfoT searchRes;
    listFoundT list_searchRes;
    char cmdInput;
    int courseId;
    hashT *hashTable;
    

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
                        printf("\nA-OK %lu, %d %d\n", studentInfo.AEM, entriesArray.numOfEntries, entriesArray.size);
                    }
                    else {
                        // Failed
                        printf("\nA-NOK %lu, %d %hu\n", studentInfo.AEM, entriesArray.numOfEntries, entriesArray.size);
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
                    printf("\nR-OK %lu, %d %hu\n", studentInfo.AEM, entriesArray.numOfEntries, entriesArray.size);
                } else {
                    // Failed
                    printf("\nR-NOK %lu, %d %hu\n", studentInfo.AEM, entriesArray.numOfEntries, entriesArray.size);
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
                deleteHash(hashTable);
                quit(&entriesArray, true);
                break;
            }
            // Add course to course list of a student
            case 'g': {

                scanf(" %lu %d", &studentInfo.AEM, &courseId);
                if (studentInfo.AEM == 0 || courseId == 0) {
                    printf("Invalid input\n");
                    clearStdin();
                }
                switch (reg(&entriesArray, studentInfo.AEM, courseId)) {
                    case '0': {
                        printf("G-OK %lu %d\n", studentInfo.AEM, courseId);
                        break;
                    }
                    case '1': {
                        printf("G-NOK %d\n", courseId);
                        break;
                    }
                    case '2': {
                        printf("G-NOK %lu\n", studentInfo.AEM);
                        break;
                    }
                    default: {
                        printf("G-NOK MEM\n");
                    }
                }
                clearStdin();
                break;
            }
            case 'x': {
                scanf(" %lu %d", &studentInfo.AEM, &courseId);
                if (studentInfo.AEM == 0 || courseId == 0) {
                    printf("Invalid input\n");
                    clearStdin();
                }
                switch (unreg(&entriesArray, studentInfo.AEM, courseId)) {
                    case '0': {
                        printf("U-OK %lu %d\n", studentInfo.AEM, courseId);
                        break;
                    }
                    case '1': {
                        printf("G-NOK %d\n", courseId);
                        break;
                    }
                    default: {
                        printf("G-NOK %lu\n", studentInfo.AEM);
                    }
                }
                clearStdin();
                break;
            }
            case 'i': {
                scanf(" %lu %d", &studentInfo.AEM, &courseId);
                if (studentInfo.AEM == 0 || courseId == 0) {
                    printf("Invalid input\n");
                    clearStdin();
                }
                list_searchRes = isreg(&entriesArray, studentInfo.AEM, courseId);

                if (list_searchRes.student == -1) printf("G-NOK %lu\n", studentInfo.AEM);
                else if (list_searchRes.empty || list_searchRes.found == false) printf("NO\n");
                else printf("YES\n");
                clearStdin();
                break;
            }
            case 'l': {
                scanf(" %lu", &studentInfo.AEM);
                if (studentInfo.AEM == 0 || courseId == 0) {
                    printf("Invalid input\n");
                    clearStdin();
                }
                if (list_courses(&entriesArray, studentInfo.AEM) == false) printf("L-NOK %lu\n", studentInfo.AEM);
                clearStdin();
                break;
            }
            case 'b': {
                hashTable = createHash(2);
                add_to_hash(hashTable, entriesArray.entry[0]);
                rmv_from_hash(hashTable, entriesArray.entry[0]);
                add_to_hash(hashTable, entriesArray.entry[2]);
                add_to_hash(hashTable, entriesArray.entry[1]);
                add_to_hash(hashTable, entriesArray.entry[3]);
                add_to_hash(hashTable, entriesArray.entry[4]);
                doReHash(&hashTable);
                break;
            }
        }
    } while(1);

	return 0;
}
