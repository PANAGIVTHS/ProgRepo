
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum {EXISTS = 2, CONTAINS = 1} find_flag;
typedef enum {FILE_END = 1, LINE_END = 0} file_status;

int append_str (char **string, const char *word) {
    char *temp;
    int string_len = 0;

    int word_len = strlen(word);
    if (*string != NULL) string_len = strlen(*string);

    temp = (char*) realloc(*string, string_len + word_len);
    if (temp == NULL) return(-1);
    *string = temp;
    (*string)[string_len + word_len - 1] = '\0';
    strcpy(&(*string)[string_len], word);

    return(0);
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

    return(eol == 1? 2 : 1);
}

int create_exec_in (int arg_fd, char ***string) {
    char *temp_str = NULL, **temp = NULL, *temp2 = NULL;
    int scan_res = 0, i;

    lseek(arg_fd, 0, 0);

    for (i = 0; 1; i++) {
        temp = (char **) realloc(*string, i*sizeof(char*) + 1);
        if (temp == NULL) return(-1);
        *string = temp;
        

        free(temp_str);
        temp_str = NULL;
        scan_res = scan(arg_fd, &temp_str);
        if (scan_res == 0) {
            (*string)[i] = NULL;
            return(i);
        }
        temp2 = (char *) malloc(strlen(temp_str));
        if (temp2 == NULL) return(-1);
        (*string)[i] = temp2;
        
        
        
        strcpy((*string)[i], temp_str);
    }

    return(i);

}

void destroy_ptr_array (void **ptr) {
    for (int i = 0; ptr[i] != NULL; i++) {
        free(ptr[i]);
    }
    free(ptr);
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

int my_write(int fd, const void* buffer, size_t count) {
    int curWritten = 0, total = 0;

    do {
        curWritten = write(fd, buffer + curWritten, count);
        if (curWritten == -1){
            return(-1);
        }
        if (curWritten == 0) return(total);
        total += curWritten;
        count -= curWritten;
    } while((count > 0));

    return(total);
}

int my_read(int fd, void* buffer, size_t count) {
    int curRead = 0, total = 0 , i=0;

    do {
        i++;
        curRead = read(fd, buffer + curRead, count);
        if (curRead == -1){
            //printf("READ :: Returning #%d: %d\n", i,-1);
            return(-1);
        }
        total += curRead;
        if (curRead == 0){
            //printf("READ :: Returning #%d: %d\n", i,total);
            return(total);
        }
        count -= curRead;
    } while(count > 0);
    
    //printf("READ :: Returning #%d: %d\n", i,total);
    return(total);
}

int line_search (int fd, const char *word, find_flag flag, file_status *status) {
    char *string = NULL;
    int scan_res = 0, total = 0, find_res = 0;

    *status = LINE_END;

    do {

        free(string);
        string = NULL;
        scan_res = scan(fd, &string);
        if (scan_res == -1) {
            break;
        }
        if (scan_res == 0) {
            *status = FILE_END;
            break;
        }
        find_res = strfind(string, word);
        if (find_res == -1) {
            free(string);
            return(-1);
        }
        if (find_res == 2 && flag == EXISTS) total++;
        else if ((find_res == 1 || find_res == 2) && flag == CONTAINS) total++;
    } while (1);

    free(string);
    return(scan_res == -1? scan_res : total);  
}

int file_search (int fd, const char *word, find_flag search) {
    int ls_res = 0, total = 0;
    file_status status;

    lseek(fd, 0, SEEK_SET);
    do {
        ls_res = line_search(fd, word, search, &status);
        if (ls_res == -1) return(-1);
        total += ls_res;
        if (status == FILE_END) break;
    } while (1);

    return(total);
}

int main(int argc, char *argv[]) {
    int penalty = 0, compiled = 1;
    int pid_err = 0, status = 0, arg_fd= 0;
    char **exec_str = NULL;
    int fd_p[2];
    pipe(fd_p);

    if (argc != 2) {
        printf("Incorrect num of arguments.\n");
        return(-2);
    }

    int fd_p1_in = open(argv[1], O_RDWR | O_EXCL | O_CREAT, 0700);

    // Already exists
    if (fd_p1_in == -1 && errno == EEXIST) {
        printf("Exists.\n");
    }
    else if (fd_p1_in == -1) {
        printf("Problem opening.\n");
        return(-2);
    }

    pid_err = fork();

    if (pid_err == 0) {
        dup2(fd_p1_in, STDERR_FILENO);
        execl("/usr/bin/gcc","gcc", "-Wall", "-g", "add.c", "-o", "add", NULL);
    }
    waitpid(pid_err, &status, 0);

    if (file_search(fd_p1_in, "error:", EXISTS) > 0) {
        penalty = -100;
        compiled = 0;
    }
    else {
        penalty = file_search(fd_p1_in, "warning:", EXISTS);
        if (penalty == -1) return(-1);
        penalty *= -5;
    }

    printf("Penalty: %d\n", penalty);
    close(fd_p1_in);
    
    int pid_p2 = fork();

    if (pid_p2 == 0) {
        // P2
        arg_fd = open("test", O_RDONLY);
        close(fd_p[0]); //close read end

        create_exec_in(arg_fd, &exec_str);
        int in_fd = open("inA", O_RDONLY);
        dup2(in_fd, STDIN_FILENO);
        dup2(fd_p[1], STDOUT_FILENO);
        // Code here
        char name[30]; 
        scanf("%s", name);
        printf("This%s", name);
    }

    int pid_p3 = fork();
        
    if (pid_p3 == 0) {
        // P3
        close(fd_p[1]); // close write end
        dup2(fd_p[0], STDIN_FILENO);
        execl("./p4diff", "p4diff", NULL);
    }
    
    close(fd_p[0]);
    close(fd_p[1]);

    waitpid(pid_p2, 0, 0);
    waitpid(pid_p3, 0, 0);

    return(0);
}

// int main () {
//     char **string = NULL;

//     int fd = open("test", O_RDONLY);

//     int j = create_exec_in(fd, &string);
//     for (int i = 0; i < j; i++) {
//         printf("%s\n", string[i]);
//     }
//     //destroy_ptr_array((void**)string);

//     return(0);
// }