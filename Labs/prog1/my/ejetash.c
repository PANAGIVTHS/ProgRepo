
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define STR_SIZE 12
#define COLS 11
#define ROWS 3

typedef enum {false, true} boolean;

// This function turns a word to lower case chars only

void str_lower (char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower(word[i]);
    }
}

// This function finds all the words (in the dictionary) that could be the target word
// returns the number of matches

int find (char dictionary[][COLS], char words[][COLS], char* target, int stringsNum) {
    int targetSize, wordsCounter = 0;
    boolean isTarget = true;

    targetSize = strlen(target);

    for (int rowsCounter = 0; rowsCounter < stringsNum; rowsCounter++) {
        // Turn string to lower case
        str_lower(dictionary[rowsCounter]);
        // If target word & dictionary word dont have the same size continue
        if (strlen(dictionary[rowsCounter]) != targetSize) {
            continue;
        }
        else {
            // Check if all the non-(_) target's indexes match the dictionary's word indexes
            for (int colCounter = 0; target[colCounter] != '\0' || colCounter < COLS; colCounter++) {
                if (target[colCounter] == '_') continue;
                // Doesnt match so the word cant be the target word
                if (target[colCounter] != dictionary[rowsCounter][colCounter]) {
                    isTarget = false;
                    break;
                }
            }
            if (isTarget) {
                // Fill the words array with the words that match
                strcpy(words[wordsCounter], dictionary[rowsCounter]);
                isTarget = true;
                wordsCounter++;
            }
        }
    }

    return(wordsCounter);
}


// This function prints a given string array

void print (char words[][COLS], int stringsNum) {
    for (int i = 0; i < stringsNum; i++) {
        printf("%s\n", words[i]);
    }
}

int main (int argc, char* argv[]) {
    char dictionary[ROWS][COLS] = {{"teApot"}, {"VesPer"}, {"keLP"}};
    char words[ROWS][COLS] = {{0}};
    char target[STR_SIZE];
    char formatStr[16];
    int stringsNum, wordsNum;

    sprintf(formatStr, "%%%ds", STR_SIZE-1);

    stringsNum = 3; // = init(dictionary);
    scanf(formatStr, target);
    wordsNum = find(dictionary, words, target, stringsNum);
    print(words, wordsNum);


    return(0);
}