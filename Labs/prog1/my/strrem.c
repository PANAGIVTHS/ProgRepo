
#include <stdio.h>
#include <string.h>

void strrem (char *word, char letter) {
    char *ptr;
    unsigned int counter = 0;

    ptr = strchr(word, letter);
    if (ptr != NULL) {
        while (1) {
            if (*ptr == letter) {
                counter++;
            }
            else if (*ptr == '\0') {
                for (; counter > 0; counter--) {
                    *(ptr-counter) = '\0';
                }
                return;
            }
            else {
                *(ptr-counter) = *ptr;
                *ptr = letter;
            }
            ptr++;
        }
    }
}

int main(int argc, char *argv[]) {
    char array[20] = "applepie";
    
    strrem(array, 'p');
    printf("%s\n", array);
    return(0);
}   
