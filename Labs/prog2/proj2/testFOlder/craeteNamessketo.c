
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
    char formatStr[100];
    char name[40];
    long unsigned int k,i;
    char name1[40];
    char cmd;


    sprintf(formatStr, "%%lu %%s %%s %%lu");
    
    do {
        scanf(" %c", &cmd);
        if (cmd == 'q') break;
        scanf(formatStr, &k, &name1, &name, &i);
        printf("%s\n", name);
    } while (1);
    printf("q");
}
