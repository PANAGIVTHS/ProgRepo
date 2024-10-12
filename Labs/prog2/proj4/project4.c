
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>

int append_str (char **string, const char *word) {
    char *temp = NULL;
    int string_len = 0;

    int word_len = strlen(word);
    if (*string != NULL) string_len = strlen(*string);

    temp = (char*) realloc(*string, string_len + word_len + 1);
    if (temp == NULL) return(-1);
    *string = temp;
    (*string)[string_len + word_len - 1] = '\0';
    strcpy(&(*string)[string_len], word);

    return(0);
}
int file_search (int fd, const char *word, find_flag search, const char *mandtry_word) {
    int ls_res = 0, total = 0, read_res = 0, found_mndtry = 0;
    file_status status;
    char *string = NULL;

    lseek(fd, 0, SEEK_SET);
    do {
        scan(fd, &string);
        if (strfind(string, mandtry_word == 1)) {
            found_mndtry = 1;
        }
        else {
            lseek(fd, -strlen(string), SEEK_CUR);
        }
        ls_res = line_search(fd, word, search, &status);
        if (ls_res == -1) return(-1);
        if (found_mndtry) total += ls_res;
        if (status == FILE_END) break;
        free(string);
        string = NULL;
        found_mndtry = 0;
    } while (1);

    return(total);
}
int main(int argc, char *argv[]) {
    char *string = NULL;

    return(0);
}