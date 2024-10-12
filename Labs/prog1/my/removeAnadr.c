
#include <stdio.h>
#include <string.h>

#define WORD_WIDTH 20

void strRemove (char *word) {
    char temp[WORD_WIDTH];
    if (*word == '\0') return;
    if ((word+1 != NULL) && (*(word+1) != '\0')) {
        strcpy(temp, (word+1));
    }
    else if (*(word+1) == '\0') {
        strcpy(temp,"\0");
    }
    strcpy(word, temp);
}

void chrrem (char *word, char letter) {
    word = strchr(word, letter);
    if(word == NULL) return;
    if(strchr(&word[1], letter) != NULL) {
        chrrem(&word[1], letter);
    }
    strRemove(word);
}

int main(int argc, char *argv[]) {
    char word[WORD_WIDTH] = {"applepop"};

    chrrem(word, 'p');
    printf("%s\n", word);
    return(0);
}   
