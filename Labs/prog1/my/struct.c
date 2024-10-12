
#include <stdio.h>
#include <string.h>


char * mystrSearch (char sentence[], char keyStrings[]) {
    char key[13] = {0};
    char *point;
    
    point = keyStrings;
    while (point != NULL) {
        sscanf(key , keyStrings);
        if (key == '\0') break;
        if (strstr(sentence, key) != NULL) {
            return(strstr(sentence, key));
        }
        else {
            point += (strchr(point, ' ') - point);
        }
    }
    return(NULL);
}


int main(int argc, char *argv[]) {

    char sentence[100] = {"I am a string WOW!"};
    char key[30] = {"WOW"};

    mystrSearch(sentence, key);

}   
