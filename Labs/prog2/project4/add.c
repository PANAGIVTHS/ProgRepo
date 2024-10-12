
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

void print (char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    while(*fmt) {
        switch (*fmt++) {
            case 's': {
                printf("s %s", va_arg(ap, char *));
            }
            case 'd': {
                printf("d %d", va_arg(ap, int));
            }
            case 'c': {
                printf("c %c", (char)va_arg(ap, int));
            }
            default: {

            }
        }
    }
    va_end(ap);
}

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

int scan (int fd, char **string) {
    char *buffer = (char*) malloc(1);
    if (buffer == NULL) return(-1);
    int buffer_size = 1;
    char *temp = NULL;
    int eol = 0;
    
    do {
        int res = read(fd, buffer + buffer_size -1, 1);
        if (res == 0) {
            free(buffer);
            return(0);
        }
        if (res == -1) {
            free(buffer);
            return(-1);
        }
        else {
            temp = (char*) realloc(buffer, buffer_size+1);
            if (temp == NULL) {
                free(buffer);
                return(-1);
            }
            buffer = temp;
            buffer_size++;
        }

    } while (buffer[buffer_size-2] != ' ' && buffer[buffer_size-2] != '\n');

    if (buffer[buffer_size-2] == '\n') eol = 1;
    *string = buffer;
    (*string)[buffer_size-2] = '\0';

    return(eol == 1? 1 : 2);
}

int create_exec_in (int arg_fd, char ***string, const char *prog_name) {
    char *temp_str = NULL, **temp = NULL, *temp2 = NULL;
    int scan_res = 0, i = 0;

    if (arg_fd == -1) return(-1);

    lseek(arg_fd, 0, 0);

    for (i = 1; 1; i++) {
        temp = (char **) realloc(*string, (i+1)*sizeof(char*));
        if (temp == NULL) return(-1);
        *string = temp;

        if (i == 1) {
            temp2 = (char *) malloc(strlen(prog_name) + 1);
            if (temp2 == NULL) return(-1);
            (*string)[0] = temp2;
            strcpy((*string)[0], prog_name);
        }
    
        free(temp_str);
        temp_str = NULL;
        scan_res = scan(arg_fd, &temp_str);
        if (scan_res == 0) {
            (*string)[i] = NULL;

            return(i);
        }
        temp2 = (char *) malloc(strlen(temp_str) + 1);
        if (temp2 == NULL) return(-1);
        (*string)[i] = temp2;

        strcpy((*string)[i], temp_str);
    }
  
    return(i);

}

int main (int argc, char *argv[]) {
    char *string1 =  "Hello";
    int i = 1;
    char *string2 = "World!";
    print("ss", string1, string2);
    return(0);
}
