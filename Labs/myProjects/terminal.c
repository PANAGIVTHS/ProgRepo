
#include <stdio.h>
#include <stdarg.h>

typedef enum {FILE_END = 0, LINE_END = 1} file_status;

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

int make_cmd_line (int fd, char ***array) {
    char ***temp = NULL;
    array = NULL;
    int scan_res = 2;

    for (int i = 0; scan_res != LINE_END && scan_res != FILE_END; i++) {
        temp = realloc(array, sizeof(char**)*(i+1));
        if (temp != NULL) array = temp;
        else return(-1);
    
        scan_res = scan(fd, array[i]);
    }

    return(0);
}

int main (int argc, char *argv[]) {

    while (1) {
        printf("term@userX: ");


    }


    return(0);
}