
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int strfind (const char *word, const char *str) {
    int str_len = strlen(str);
    int word_len = strlen(word);
    bool same_word = true;
    char *temp2, *temp3;
    
    char *temp = (char*) malloc(word_len+1);
    if (temp == NULL) return(-1);
    temp3 = temp;

    strcpy(temp, word);

    if (word_len < str_len) {
        free(temp3);
        return(0);
    }

    do {
        temp2 = strchr(temp, str[0]);
        if (temp2 == NULL) {
            free(temp3);
            return(0);
        }
        else {
            temp = temp2;
        }
        if (temp[1] == '\0') {
            free(temp3);
            return(1);
        }
        int i;
        for(i = 1; i < str_len; i++) {
            if(temp[i] != str[i]) {
                same_word = false;
                break;
            }
        }
        if (!same_word) {
            free(temp3);
            return(0);
        }
        if ((same_word) && (str_len == word_len)) {
            free(temp3);
            return(2);
        }
        else if ((i >= str_len-1)) {
            free(temp3);
            return(1);
        }
        else {
            temp++;
        }
    
    } while (1);

    free(temp3);
    return(0);
}

void clearStdin() {
    char binInput;

    do {
        scanf("%c", &binInput);
    } while (binInput != '\n');
}

int getstr (int fd, char **string) {
    int buffer_size = 1;
    char *temp;

    char *buffer = (char*) malloc(sizeof(char));
    if (buffer == NULL) return(-1);

    do {
        int res = read(fd, buffer + buffer_size -1, sizeof(char));
        if (res != 0) {
            temp = (char*) realloc(buffer, buffer_size + sizeof(char));
            if (temp == NULL) {
                free(buffer);
                return(-1);
            }
            else {
                buffer = temp;
                buffer_size++;
            }
        }
        else {
            return(0);
        }
    } while (buffer[buffer_size-2] != ' ' && buffer[buffer_size-2] != '\n');
    *string = (char*) malloc(buffer_size);
    strncpy(*string, buffer, buffer_size-1);
    string[buffer_size-1] = '\0';

    return(0);
}

int stdin_search (const char *string ) {
    char *temp_in = NULL;
    int found_counter = 0;

    do {
        if (scanf("%ms", &temp_in) == EOF) break;
        // Does the line start with progname.
        if (strfind(temp_in, "add.c") != 0) {
            free(temp_in);
            temp_in = NULL;
            if (scanf("%ms", &temp_in) == EOF) break;
            if (strfind(temp_in, string) != 0) {
                found_counter++;
            }
            free(temp_in);
            temp_in = NULL;
        }
        clearStdin();
    } while (1);

    return(found_counter);
}

int main () {
    int i, j , k, error;
    //char *string = NULL;
    // int fd = open("temp", O_RDONLY);
    // dup2(fd, STDIN_FILENO);
    // getstr(fd, &string);
    // printf("%s\n", string);
    char name[40];
    scanf("%s", name);
    // for (int i = -10000; i < 10000; i++) {
    //     name[i] = name[i];
    // }
    printf("%s\n", name);
    test(" error: ");
    
    // int i ,j;
    // int fd = open("temp", O_RDONLY);
    // int dupl = dup(STDIN_FILENO);
    // dup2(fd, STDIN_FILENO);
    // printf("%d\n", stdin_search("error:"));
    // scanf("%d %d", &i, &j);
    // dup2(dupl, STDIN_FILENO);
    // if (scanf("%d %d", &i, &j) == EOF) printf("WOW");
    // scanf("%d %d", &i, &j);
    // scanf("%d %d", &i, &j);
    // fd = open("temp1", O_RDONLY);
    // dup2(fd, STDIN_FILENO);
    // printf("%d\n", stdin_search("error:"));
    
    return(0);
}