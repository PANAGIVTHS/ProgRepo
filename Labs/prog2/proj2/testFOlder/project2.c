/* ONOMATEPWNYMO: TsogkasPanagiotis Nikolaos
AEM: 03672
*/

/* 
    This programme stores student entries in a Database which can be modified (add, remove entries)
    Entries can also be modified, sorted, listed on screen
    Commands: p (print), c (clear db), a (add entry), r (remove entry), m (modify entry)
    q (exit programme), s (sort entries), f (find entry), i (find if student is registered to a course)
    r (register a student to a course), u (unregister a student from a course)

    The programme also contains a hashTable that stores students based on alphabetical order
    Commands: n (find a student by sir-name), t (print hashtable) 

    Status: 0,1 (Normal exit), 42 (wrong arguments), 43 (database Initial malloc fail)
*/

#include"project2.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// enum for realloc increase / decrease size
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
bool add_to_hash (hashT *hashTable, studentEntryT *student);
bool rmv_from_hash (hashT *hashTable, studentEntryT *student);
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


// hash function for hash table

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
        for (result.index = 0, result.comps = 0; (result.index < entriesArray->numOfEntries) && (result.index < entriesArray->size); result.index++) {
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

// Do malloc for a list's head
// handles failure
// returns success / failure

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

// Check if a student is registered to a course
// return all search info

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
    if ((entriesArray->entry[result_find.index]->courses == NULL)) {
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


// register a student to a course
// returns status 0: success 1: course exits 2: No student

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
        if (listMalloc(&tempNode)) {
            tempNode->courseId = courseId;
            tempNode->next = entriesArray->entry[searchRes.student]->courses;
            entriesArray->entry[searchRes.student]->courses = tempNode;
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


// unregister a student from a course
// returns status 0: success 1: No student 2: student found but no course

int unreg (databaseT *entriesArray, unsigned long int targetAEM, int courseId) {
    listFoundT searchRes;
    list *tempNode;

    searchRes = isreg(entriesArray, targetAEM, courseId);
    if (searchRes.student == -1) return(1);
    if (searchRes.empty == true) return(2);
    if (searchRes.found == false) return(2);
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

    if (entriesArray->appendSize == 0) return(false);
    // No need to change size already big enough
    if ((scaler == UP) && (entriesArray->size > entriesArray->numOfEntries)) return(false);
    // Cant decrease size of 0
    if ((scaler == DOWN) && (entriesArray->size - entriesArray->appendSize < 0)) return(false);
    // Not enough free entries to decrease size
    if ((scaler == DOWN) && (entriesArray->size - entriesArray->numOfEntries < entriesArray->appendSize)) return(false);
    if ((scaler == DOWN) && (entriesArray->size - entriesArray->appendSize == 0)) {
        free(entriesArray->entry);
        entriesArray->entry = NULL;
        entriesArray->size = 0;
        return(true);
    }

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

bool addStudent (databaseT *entriesArray, studentEntryT studentInfo, hashT *hashTable) {

    // If false then add a student
    if (findStudent(entriesArray, studentInfo.AEM).isFound == false) {
        // If no space realloc will be executed
        doRealloc(entriesArray, UP);
        // If entry == NULL then realloc failed when array size was 0
        // so dont check entry->..
        if ((entriesArray->appendSize != 0) && (entriesArray->entry != NULL)) {
            // Create memory
            entriesArray->entry[entriesArray->numOfEntries] = (studentEntryT*) malloc(sizeof(studentEntryT));
            if (entriesArray->entry[entriesArray->numOfEntries] != NULL) {
                *(entriesArray->entry[entriesArray->numOfEntries]) = studentInfo;
                add_to_hash(hashTable, entriesArray->entry[entriesArray->numOfEntries]);
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


void destroyList (list *list) {
    if (list != NULL) {
        destroyList(list->next);
        free(list);
        list = NULL;
    }
}

// This function removes an entry from a database with a specified AEM value decerasing it's size if needed
// Returns true if removal was successful and false if it wasnt 

bool rmvStudent (databaseT *entriesArray, unsigned long int targetAEM, hashT *hashTable) {
    findInfoT searchRes;
    int lastStudent = entriesArray->numOfEntries - 1;

    searchRes = findStudent(entriesArray, targetAEM);
    
    // Check if student exists so as to remove him
    if (searchRes.isFound == true) {
        // Remove from hash first
        rmv_from_hash(hashTable, entriesArray->entry[searchRes.index]);

        destroyList(entriesArray->entry[searchRes.index]->courses);
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
    if (argc != 4) {
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
    for (int i = 0; (i < entriesArray->numOfEntries) && (i < entriesArray->size); i++) {
        printf("%lu %s %s %hu\n", entriesArray->entry[i]->AEM, entriesArray->entry[i]->studentID.firstName,
                entriesArray->entry[i]->studentID.lastName, entriesArray->entry[i]->coursesFailed);
    }
}

// This function clears the database and frees all memory 

void clear (databaseT *entriesArray) {
    for (int i = 0; (i < entriesArray->numOfEntries) && (i < entriesArray->size); i++) {
        destroyList(entriesArray->entry[i]->courses);
        free(entriesArray->entry[i]);
    }
    if (entriesArray->appendSize != 0) entriesArray->size = 0;
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

// This function initialises the sentinel of a list

void init_lnkList (linked_list *sentinel) {
    sentinel->studentPtr = NULL;
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
}

// This functions returns the alphabetically smallest word (1 = first, 2 = second)
// If they are the same returns 0

int compareWords (char *word, char *word_to_cmpr) {
    int i;

    for (i = 0; (word[i] != '\0') && (word_to_cmpr[i] != '\0'); i++) {
        if (word[i] < word_to_cmpr[i]) return(1);
        else if (word[i] > word_to_cmpr[i]) return(2);
        else continue;
    }
    // Before deciding check if one word's bigger than the other
    if (word[i] != '\0') return(2);
    if (word_to_cmpr[i] != '\0') return(1);
    // Same word
    return(0);
}

// Find a student in a linked list
// return pointer to that student

linked_list *find_lnkList (linked_list *sentinel, studentEntryT *student) {
    linked_list *curr;

    sentinel->studentPtr = student;

    for (curr = sentinel->next; compareWords(curr->studentPtr->studentID.lastName, student->studentID.lastName) == 1; curr = curr->next);
    while ((compareWords(curr->studentPtr->studentID.lastName, student->studentID.lastName) == 0) && (curr->studentPtr->AEM < student->AEM)) {
        curr = curr->next;
    }

    sentinel->studentPtr = NULL;

    return(curr);
}

// Add a vertex to the list and fill it with student pointer

bool add_lnkList (linked_list *sentinel, studentEntryT *student) {
    linked_list *result, *temp;
    

    result = find_lnkList(sentinel, student);
    if (result->studentPtr == student) {
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

// Remove a specified (through student pointer) vertex from a linked list
// Returns success / failure status

bool rmv_lnkList (linked_list *sentinel, studentEntryT *student) {
    linked_list *result, *temp;
    

    result = find_lnkList(sentinel, student);
    if (result->studentPtr != student) {
        // Not Found
        return(false);
    }
    // Found
    temp = result;
    temp->next->prev = temp->prev;
    result->prev->next = temp->next;
    free(temp);

    return(true);
}

// Initialise a given hashTable

void init_hash (hashT *hashTable, int initialSize) {
    hashTable->elements = 0;
    hashTable->initialSize = initialSize;
    hashTable->size = initialSize;
    for (int i = 0; i < hashTable->size; i++) {
        init_lnkList(hashTable->lnk_list[i].sentinel);
        hashTable->lnk_list[i].list_size = 0;
    }
}

// Create a hashTable
// Reterns pointer to the newly created hash 
// or NULL if failed to create

hashT *createHash (int size) {
    hashT *temp;

    temp = (hashT*) malloc(sizeof(hashT));
    if (temp == NULL) return(NULL);
    temp->lnk_list = (bucketT*) malloc(size * sizeof(bucketT));
    if (temp->lnk_list == NULL) return(NULL);

    for (int i = 0; i < size; i++) {
        temp->lnk_list[i].sentinel = (linked_list*) malloc(sizeof(linked_list));
        // Failed to create start deletion
        if (temp->lnk_list[i].sentinel == NULL) {
            for (int j = 0; j < i; j++) {
                free(temp->lnk_list[i].sentinel);
            }
            free(temp->lnk_list);
            free(temp);
            return(NULL);
        }
    }

    init_hash(temp, size);
    
    return(temp);
}

// Add a new student to a hashTable 
// returns success / failure status

bool add_to_hash (hashT *hashTable, studentEntryT *student) {
    bool was_added;
    int index;

    index = hash(student->studentID.lastName)%hashTable->size;
    was_added = add_lnkList(hashTable->lnk_list[index].sentinel, student);
    // Found so return
    if (was_added == false) return(false);
    hashTable->lnk_list[index].list_size++;
    hashTable->elements++;
    
    return(true);
}

// Delete a given list

void deleteList (linked_list *list) {
    
    if (list == NULL) return;
    linked_list *curr = list->prev;
    
    if (curr != list) {
        do {
            curr = curr->prev;
            free(curr->next);
        } while (curr != list);
    }
}

// Delete a hash table

void deleteHash (hashT *hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        deleteList(hashTable->lnk_list[i].sentinel);
        free(hashTable->lnk_list[i].sentinel);
    }
    free(hashTable->lnk_list);
    hashTable->elements = 0;
    free(hashTable);
}

// This function checks if rehashing is needed and does so if true
// returns success / failure status

bool doReHash (hashT **hashTable) {
    hashT *newTable;

    if (((double)(*hashTable)->elements / (double)(*hashTable)->size <= LOW_LOAD) && ((*hashTable)->size / 2 >= (*hashTable)->initialSize)) {
        newTable = createHash((*hashTable)->size/2);
        newTable->initialSize = (*hashTable)->initialSize;
    }
    else if (((double)(*hashTable)->elements / (double)(*hashTable)->size >= HIGH_LOAD)) {
        newTable = createHash((*hashTable)->size*2);
        newTable->initialSize = (*hashTable)->initialSize;
    }
    else {
        return(false);
    }

    if(newTable == NULL) return(false);
    for (int i = 0; i < (*hashTable)->size; i++) {
        if (i < newTable->size) newTable->lnk_list[i].list_size = 0;
        for (linked_list *curr = (*hashTable)->lnk_list[i].sentinel->next; curr != (*hashTable)->lnk_list[i].sentinel; curr = curr->next) {
            add_to_hash(newTable, curr->studentPtr);
        } 
    }
    newTable->initialSize = (*hashTable)->initialSize;
    deleteHash((*hashTable));
    (*hashTable) = newTable;
    
    return(true);
}

// Remove a student from a hashTable

bool rmv_from_hash (hashT *hashTable, studentEntryT *student) {
    bool was_removed;
    int index;

    index = hash(student->studentID.lastName)%hashTable->size;
    was_removed = rmv_lnkList(hashTable->lnk_list[index].sentinel, student);
    // Found so return
    if (was_removed == false) return(false);
    hashTable->lnk_list[index].list_size--;
    hashTable->elements--;

    return(true);
}

// Find a student by sir-name in a hash table 

int find_by_name (hashT *hashTable, studentEntryT *student) {
    linked_list *curr;
    int comps = 0, index;
    bool found = false;
    int word_comp_result;

    index = hash(student->studentID.lastName)%hashTable->size;

    hashTable->lnk_list[index].sentinel->studentPtr = student;
    curr = hashTable->lnk_list[index].sentinel->next;
    do {
        if (curr == hashTable->lnk_list[index].sentinel) break;
        word_comp_result = compareWords(curr->studentPtr->studentID.lastName, student->studentID.lastName);
        comps++;
        if (word_comp_result == 2) break;
        
        // Found
        if (word_comp_result == 0) {
            if (curr == hashTable->lnk_list[index].sentinel) break;
            //if (found == false) printf("\nN-OK %s\n", student->studentID.lastName);
            //printf("%s %lu %hu\n", curr->studentPtr->studentID.firstName, curr->studentPtr->AEM, curr->studentPtr->coursesFailed);
            found = true;
        }
        curr = curr->next;
    } while (1);
    if (found == false) {
        //printf("\nN-NOK %s\n", student->studentID.lastName);
    }
    hashTable->lnk_list[index].sentinel->studentPtr = NULL;


    return(comps);
}

// Finds the largest bucket in a hashTable
// returns pointer to that bucket

bucketT *find_largest (hashT *hashTable) {
    bucketT *largest_temp;
    largest_temp = &(hashTable->lnk_list[0]);
    for (int i = 0; (i < hashTable->size-1) && (hashTable->elements > 0); i++) {
        if (largest_temp->list_size < hashTable->lnk_list[i+1].list_size) {
            largest_temp = &hashTable->lnk_list[i+1];
        }
    }
    return(largest_temp);
}

// Prints the buckets of the hash table

void print_by_name (hashT *hashTable) {
    linked_list *curr;
    bucketT *tempBucket = find_largest(hashTable);

    printf("\n##\n");
    printf("%d %d %.2lf %d\n", hashTable->size, hashTable->elements, (double)hashTable->elements / (double)hashTable->size, tempBucket == NULL? 0 : tempBucket->list_size);
    for (int i = 0; i < hashTable->size; i++) {
        printf("%d %d", i, hashTable->lnk_list[i].list_size);
        if (hashTable->elements != 0) {
            for (curr = hashTable->lnk_list[i].sentinel->next; curr != hashTable->lnk_list[i].sentinel; curr = curr->next) {
                printf(" [ %lu %s %s %hu ]", curr->studentPtr->AEM, curr->studentPtr->studentID.firstName, curr->studentPtr->studentID.lastName, curr->studentPtr->coursesFailed);
            }
        }
        printf("\n\n");
    }
}

// Clears the hash

void clearHash (hashT **hashTable) {
    hashT *tempHash = createHash((*hashTable)->initialSize);
    deleteHash(*hashTable);
    *hashTable = tempHash;
}

int main (int argc, char *argv[]) {
    bool isNameTrimmed, isLastNameTrimmed;
    char *firstNameTemp, *lastNameTemp;
    studentEntryT studentInfo;
    listFoundT list_searchRes;
    databaseT entriesArray;
    findInfoT searchRes;
    hashT *hashTable;
    char cmdInput;
    int courseId;
    

    studentInfo.AEM = 0;
    studentInfo.coursesFailed = 0;
    studentInfo.courses = NULL;

    // Initialise Database
    init_db(&entriesArray, argc, argv);
    // Create hash
    hashTable = createHash(atoi(argv[3]));
    // hash failed
    if (hashTable == NULL) exit(43);
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
                    if (addStudent(&entriesArray, studentInfo, hashTable)) {
                        // Successful
                        /* Rehash if needed*/
                        doReHash(&hashTable);
                        //printf("\nA-OK %lu, %d %d\n", studentInfo.AEM, entriesArray.numOfEntries, entriesArray.size);
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
                
                break;
            }
            // Remove a student enrty
            case 'r': {
                scanf("%lu", &studentInfo.AEM);
                if (rmvStudent(&entriesArray, studentInfo.AEM, hashTable)) {
                    // Successful 
                    /* Rehash if needed*/
                    doReHash(&hashTable);
                    printf("\nR-OK %lu, %d %hu\n", studentInfo.AEM, entriesArray.numOfEntries, entriesArray.size);
                } else {
                    // Failed
                    printf("\nR-NOK %lu, %d %hu\n", studentInfo.AEM, entriesArray.numOfEntries, entriesArray.size);
                }

                // Clear untli \n
                
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
                clearHash(&hashTable);
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
                    printf("\nInvalid input.\n");
                    
                    break;
                }
                switch (reg(&entriesArray, studentInfo.AEM, courseId)) {
                    case 0: {
                        printf("\nG-OK %lu %d\n", studentInfo.AEM, courseId);
                        break;
                    }
                    case 1: {
                        printf("\nG-NOK %d\n", courseId);
                        break;
                    }
                    case 2: {
                        printf("\nG-NOK %lu\n", studentInfo.AEM);
                        break;
                    }
                    default: {
                        printf("\nG-NOK MEM\n");
                    }
                }
                
                break;
            }
            // do unreg
            case 'u': {
                scanf(" %lu %d", &studentInfo.AEM, &courseId);
                if (studentInfo.AEM == 0 || courseId == 0) {
                    printf("\nInvalid input.\n");
                    
                    break;
                }
                switch (unreg(&entriesArray, studentInfo.AEM, courseId)) {
                    case 0: {
                        printf("\nU-OK %lu %d\n", studentInfo.AEM, courseId);
                        break;
                    }
                    case 1: {
                        printf("\nU-NOK %lu\n", studentInfo.AEM);
                        break;
                    }
                    default: {
                        printf("\nU-NOK %d\n", courseId);
                    }
                }
                break;
            }
            // Check if a student is registered in a given course
            case 'i': {
                scanf(" %lu %d", &studentInfo.AEM, &courseId);
                if (studentInfo.AEM == 0 || courseId == 0) {
                    printf("\nInvalid input.\n");
                }
                list_searchRes = isreg(&entriesArray, studentInfo.AEM, courseId);
                if (list_searchRes.student == -1) printf("\nI-NOK %lu\n", studentInfo.AEM);
                else if (list_searchRes.empty || list_searchRes.found == false) printf("\nNO\n");
                else printf("\nYES\n");
                
                break;
            }
            // list courses of a given student
            case 'l': {
                scanf(" %lu", &studentInfo.AEM);
                if (studentInfo.AEM == 0) {
                    printf("\nInvalid input.\n");
                }
                if (list_courses(&entriesArray, studentInfo.AEM) == false) printf("\nL-NOK %lu\n", studentInfo.AEM);
                break;
            }
            // Print all students with a given sir-name
            case 'n': {
                scanf("%ms", &lastNameTemp);
                // trimm names if needed
                if (trimWord(&lastNameTemp, NAMESIZE)) printf("\nTrimmed name(s).\n");
                strcpy(studentInfo.studentID.lastName, lastNameTemp);
                printf("%d\n",find_by_name(hashTable,&studentInfo));
                free(lastNameTemp);
                break;
            }
            // Print the state of a hashTbale
            case 't': {
                print_by_name(hashTable);
                break;
            }
            // Wrong Input
            default: {
                // do nothing
            }
        }
        // clear stdin
        clearStdin();
    } while(1);

	return 0;
    // end 
}
