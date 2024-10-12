
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
#include <ctype.h>

typedef enum {EXISTS = 2, CONTAINS = 1} find_flag;
typedef enum {FILE_END = 1, LINE_END = 0} file_status;

int append_str (char **string, const char *word) {
    char *temp;
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
        if (scan_res == 1) break;
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

int file_search (int fd, const char *word, find_flag search, const char *mandtry_word) {
    int ls_res = 0, total = 0, found_mndtry = 0;
    file_status status;
    char *string = NULL;

    lseek(fd, 0, SEEK_SET);
    do {
        scan(fd, &string);
        if (string == NULL) break;
        if (strfind(string, mandtry_word) == 1 || strcmp(string, "collect2:") == 0) {
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

static void myhandler (int sig) {
    write(STDERR_FILENO,"got SIGALRM\n",12);
}

int main(int argc, char *argv[]) {
    int pid_err = 0, status = 0, status2 = 0, arg_fd= 0, out_score = 0;
    int penalty = 0, compiled = 1, sig_exit = 0;
    int comp_score = 0, term_score = 0;
    struct sigaction action = {{0}};
    struct itimerval t = {{0}};
    char *prog_name = NULL;
    char **exec_str = NULL;
    int fd_p[2];
    pipe(fd_p);

    action.sa_handler = myhandler;
    sigaction(SIGALRM, &action, NULL);

    if (argc != 3) {
        printf("Incorrect num of arguments.\n");
        return(-2);
    }
    
    append_str(&prog_name, argv[1]);
    append_str(&prog_name, ".err");
    int fd_p1_in = open(prog_name, O_RDWR | O_EXCL | O_CREAT, 0700);
    free(prog_name);
    prog_name = NULL;

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
        append_str(&prog_name, argv[1]);
        append_str(&prog_name, ".c");
        execl("/usr/bin/gcc","gcc", "-Wall", "-g", prog_name, "-o", argv[1], NULL);
    }
    waitpid(pid_err, &status, 0);

    if (file_search(fd_p1_in, "error:", EXISTS, argv[1]) > 0) {
        penalty = -100;
        compiled = 0;
    }
    else {
        penalty = file_search(fd_p1_in, "warning:", EXISTS, argv[1]);
        if (penalty == -1) return(-1);
        penalty *= -5;
    }

    close(fd_p1_in);

    if (compiled) {
        t.it_interval.tv_sec = atoi(argv[2]);
        t.it_interval.tv_usec = 0;
        t.it_value.tv_sec = atoi(argv[2]);
        t.it_value.tv_usec = 0;

        setitimer(ITIMER_REAL, &t, NULL);

        int pid_p2 = fork();

        if (pid_p2 == 0) {
            // P2
            append_str(&prog_name, argv[1]);
            append_str(&prog_name, ".args");
            arg_fd = open(prog_name, O_RDONLY);
            close(fd_p[0]); //close read end
            free(prog_name);
            prog_name = NULL;

            create_exec_in(arg_fd, &exec_str);
            append_str(&prog_name, argv[1]);
            append_str(&prog_name, ".in");
            int in_fd = open(prog_name, O_RDONLY);
            free(prog_name);
            dup2(in_fd, STDIN_FILENO);
            dup2(fd_p[1], STDOUT_FILENO);
            close(fd_p[1]);
            close(in_fd);
            // Code here
            execv(argv[1], exec_str);
        }

        int pid_p3 = fork();

        if (pid_p3 == 0) {
            // P3
            close(fd_p[1]); // close write end
            dup2(fd_p[0], STDIN_FILENO);
            close(fd_p[0]);
            append_str(&prog_name, argv[1]);
            append_str(&prog_name, ".out");
            execl("./p4diff", "p4diff", prog_name, NULL);
        }

        close(fd_p[0]);
        close(fd_p[1]);

        if (waitpid(pid_p2, &status2, 0) == -1) {
            if (errno == EINTR) {
                sig_exit = 100;
                kill(pid_p2, SIGKILL);
            }
        }
        if (WTERMSIG(status2) == SIGSEGV || WTERMSIG(status2) == SIGABRT || WTERMSIG(status2) == SIGBUS) {
            sig_exit = 15;
        }

        if (waitpid(pid_p3, &status, 0) == -1) {
            if (errno != EINTR) {
                return(-1);
            }
            else {
                kill(pid_p3, SIGKILL);
            }
        }

    }
    out_score = (compiled ? WEXITSTATUS(status) : 0);
    comp_score = (compiled ? penalty : -100);
    term_score = (sig_exit == 100 ? -sig_exit : 0);
    printf("Compilation: %d\n", comp_score);
    printf("Termination: %d\n", term_score);
    printf("Output: %d\n", out_score);
    printf("Memory access: %d\n", sig_exit != 100 ? -sig_exit : 0);
    printf("Score: %d\n", (0 > (out_score + comp_score + term_score - sig_exit) ? 0 : (out_score + comp_score + term_score - sig_exit)));

    exit(0);
}

// int main () {
//     int fd = open("add.err", O_RDONLY);

//     printf("%d",file_search(fd, "erro ", CONTAINS, "add"));

//     exit(0);
// }