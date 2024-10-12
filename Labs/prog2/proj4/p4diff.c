
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

int main (int argc, char *argv[]) {
    int total = 0;
    int fd2 = STDIN_FILENO;

    if (argc != 2) return(-2);

    int fd = open(argv[1], O_RDONLY);
    int my_len = 0, res = 0, res1 = 0;

    char my_out[64];
    char prof_out[64];

    do {
        res = my_read(fd2, &my_out, 64);
        res1 = my_read(fd, &prof_out, 64);
        if (res == -1 || res1 == -1) return(-1);
        if (res == 0 || res1 == 0) break;

        my_len += res;
        for (int i = 0; i < (res < res1 ? res : res1); i++) {
            if (my_out[i] == prof_out[i]) total++;
        }
    } while (1);
    
    int out_len = lseek(fd, 0, SEEK_END);

    if ((out_len == 0) && (my_len == 0)) return(100);
    return((total*100)/(my_len > out_len? my_len : out_len));
}