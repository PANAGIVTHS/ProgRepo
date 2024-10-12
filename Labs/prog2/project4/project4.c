
// This programme is a mini autolab 
// calls another programme named p4diff to grade programme output

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
typedef enum {OUTPUT = 0, TERMINATION, COMPILE, MEM, FINAL} scores;

//This function appends a string to another 
//Returns 0 = success , -1 = failure

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

// basic scanf(%s) functionality
// returns 0 if read 0, -1 if failed, 1 if end of line, 2 if success

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

// Creates the string needed for execv
// returns 0, -1 success, fail

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

// destroy array of pointers

void destroy_ptr_array (void **ptr) {
    for (int i = 0; ptr[i] != NULL; i++) {
        free(ptr[i]);
    }
    free(ptr);
}

// Find a given string in a word
// 1 if found 2 if word is string 0 not found -1 fail

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

// Safe write. *WIll* write count bytes if they exist

int my_write(int fd, const void* buffer, size_t count) {
    int curWritten = 0, total = 0;

    do {
        curWritten = write(fd, buffer + total, count);
        if (curWritten == -1){
            return(-1);
        }
        if (curWritten == 0) return(total);
        total += curWritten;
        count -= curWritten;
    } while((count > 0));

    return(total);
}

// Safe read. *WIll* read count bytes if they exist

int my_read(int fd, void* buffer, size_t count) {
    int curRead = 0, total = 0 , i=0;

    do {
        i++;
        curRead = read(fd, buffer + total, count);
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

// Searches a line of a file for a string
// takes flag {EXISTS, CONTAINS}, 
//   CONTAINS: check if the string is anywhere in the line
//   EXISTS: check if there exists as one word
// status flag: FILE_END, LINE_END (returns file status)
// RETURNS: -1 failure, or number of instances 

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

// Line search until FILE_END, keyword needs to appear at the start of a line 
// for word instance to be counted

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

// Sets the timer variable for ALARM

void setTimeVar (struct itimerval *t, int seconds) {
    t->it_interval.tv_sec = seconds;
    t->it_interval.tv_usec = 0;
    t->it_value.tv_sec = seconds;
    t->it_value.tv_usec = 0;
}

// Prints score

void printScore (int score[5]) {
    printf("\nCompilation: %d\n", score[COMPILE]);
    printf("\nTermination: %d\n", score[TERMINATION]);
    printf("\nOutput: %d\n", score[OUTPUT]);
    printf("\nMemory access: %d\n", score[MEM]);
    printf("\nScore: %d\n", score[FINAL]);
}

int main(int argc, char *argv[]) {
    int pid_err = 0, status = 0, status2 = 0, arg_fd= 0;
    int penalty = 0, compiled = 1, sig_exit = 0, file_srch_res = 0;
    int score[5];
    struct sigaction action = {{0}};
    struct itimerval t = {{0}};
    char *prog_name = NULL;
    char **exec_str = NULL;
    int fd_p[2];
    pipe(fd_p);

    action.sa_handler = myhandler;
    sigaction(SIGALRM, &action, NULL);

    // If not enough arguments then exis
    if (argc != 6) {
        fprintf(stderr, "Incorrect num of arguments.\n");
        return(-2);
    }

    // Create executable's name string
    char *obj_prog = (char*) malloc(strlen(argv[1]) - 1);
    if (obj_prog == NULL) {
        perror("Malloc(#1 main:) failed:");
        return(-1);
    }
    strncpy(obj_prog, argv[1], strlen(argv[1]) - 2);
    obj_prog[strlen(argv[1]) - 2] = '\0';

    // Create a file named: <progname>.err
    append_str(&prog_name, obj_prog);
    append_str(&prog_name, ".err");
    int fd_err = open(prog_name, O_RDWR | O_EXCL | O_CREAT, 0700);
    free(prog_name);
    free(obj_prog);
    obj_prog = NULL;
    prog_name = NULL;

    // Already exists
    if (fd_err == -1 && errno == EEXIST) {
        fprintf(stderr, "Exists.\n");
    }
    else if (fd_err == -1) {
        // Problem opening.
        return(-2);
    }

    // --FORK(1)--
    // Compile programme that needs testing via gcc

    pid_err = fork();

    if (pid_err == 0) {
        // Set gcc out > <proganme>.err file    
        dup2(fd_err, STDERR_FILENO);

        // Create executable name / PATH
        char *obj_prog = (char*) malloc(strlen(argv[1]) - 1);
        if (obj_prog == NULL) {
            perror("Malloc(#2 main:) failed:");
            return(-1);
        }
        strncpy(obj_prog, argv[1], strlen(argv[1]) - 2);
        obj_prog[strlen(argv[1]) - 2] = '\0';

        // Compile
        execl("/usr/bin/gcc","gcc", "-Wall", "-g", argv[1], "-o", obj_prog, NULL);
    }
    // --FORK(1)_END-- 

    // Wait for child to return value
    if (waitpid(pid_err, &status, 0) == -1) {
        // Something went wrong. compilation failed
        if (WIFSIGNALED(status)) {
            compiled = 0;
        } 
    }

    // Search file for string "error:", "warning:", line has to start from string argv[1] = "<progname>,c"
    file_srch_res = file_search(fd_err, "error:", EXISTS, argv[1]);
    
    if (file_srch_res > 0) {
        penalty = -100;
        compiled = 0;
    }
    else if (file_srch_res == -1) {
        perror("file_srch (#1) failed: ");
        return(-1);
    }
    else {
        penalty = file_search(fd_err, "warning:", EXISTS, argv[1]);
        if (penalty == -1) {
            perror("file_srch (#2) failed: ");
            return(-1);
        }
        penalty *= -5;
    }

    // Done with <progname>.err 
    close(fd_err);


    // Compilation SUCCESS
    if (compiled) {
        status = 0;

        setTimeVar(&t, atoi(argv[5]));
        if (setitimer(ITIMER_REAL, &t, NULL) == -1) {
            // Failed to set tiemr
            perror("Timer failed (#1):");
            return(-1);
        }
        
        // --FORK(2)--
        // Run the executable of <progname>.c

        int pid_p2 = fork();

        if (pid_p2 == 0) {
            
            // Open argument file
            arg_fd = open(argv[2], O_RDONLY);
            if (arg_fd == -1) {
                // Cannot open
                return(-1);
            }
            close(fd_p[0]); //close read end

            // Create name of executable <progname>
            char *obj_prog = (char*) malloc(strlen(argv[1]) - 1);
            if (obj_prog == NULL) {
                perror("Malloc(#3 main:) failed:");
                return(-1);
            }
            strncpy(obj_prog, argv[1], strlen(argv[1]) - 2);
            obj_prog[strlen(argv[1]) - 2] = '\0';

            // Create array of arguments
            create_exec_in(arg_fd, &exec_str, obj_prog);
            
            // Open <progname>.in (input file)
            int in_fd = open(argv[3], O_RDONLY);
            if (in_fd == -1) {
                // return + free any allocated memory
                exit(-1);
            }

            // Set input > input file and output > pipe write end
            dup2(in_fd, STDIN_FILENO);
            dup2(fd_p[1], STDOUT_FILENO);
            close(fd_p[1]);
            close(in_fd);
            
            // Run programme
            execv(obj_prog, exec_str);
        }
        // --FORK(2)_END--

        // --FORK(3)--
        // Run p4diff is the c equivalent of linux's diff command
        int pid_p3 = fork();

        if (pid_p3 == 0) {
            close(fd_p[1]); // close write end
            dup2(fd_p[0], STDIN_FILENO);
            close(fd_p[0]);

            // Run p4diff with input the <progname>'s output
            execl("./p4diff", "p4diff", argv[4], NULL);
        }
        // --FORK(3)_END--

        // Main doesnt care about pipe
        close(fd_p[0]);
        close(fd_p[1]);

        // Check is child was interrupted 
        if (waitpid(pid_p2, &status2, 0) == -1) {
            // Main has been interrupted. The child's time has come :)
            if (errno == EINTR) {
                sig_exit = 100;
                // Kill child 
                kill(pid_p2, SIGKILL);
                errno = 0;
            }
        }
        else {
            // Stop timer
            setTimeVar(&t, 0);
            if (setitimer(ITIMER_REAL, &t, NULL) == -1) {
                perror("Reset Timer failed (#2):");
                return(-1);
            }
        }

        // If child returned check if signaled
        if (errno != EINTR && (WIFSIGNALED(status2)) && (WTERMSIG(status2) == SIGSEGV || WTERMSIG(status2) == SIGABRT || WTERMSIG(status2) == SIGBUS)) {
            sig_exit = 15;
        }

        // If child Interrupted something went wrong
        if (waitpid(pid_p3, &status, 0) == -1) {
            if (errno != EINTR) {
                return(-1);
            }
            else {
                // Kill child :)
                kill(pid_p3, SIGKILL);
            }
        }

    }

    // Set scores 
    score[OUTPUT] = (compiled ? WEXITSTATUS(status) : 0);
    score[COMPILE] = (compiled ? penalty : -100);
    score[TERMINATION] = (sig_exit == 100 ? -sig_exit : 0);
    score[MEM] = sig_exit != 100 ? -sig_exit : 0;
    score[FINAL] = (0 > (score[OUTPUT] + score[COMPILE] + score[TERMINATION] - sig_exit) ? 0 : (score[OUTPUT] + score[COMPILE] + score[TERMINATION] - sig_exit));
    
    // Print scores
    printScore(score);

    exit(0);
}