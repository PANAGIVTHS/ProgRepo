
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
    char formatStr[100];
    char name[40];

    sprintf(formatStr, "%%s");
    
    do {
        scanf(formatStr, &name);
        if (strcmp("q", name)==0) break;
        printf("n %s\n", name);
    } while (1);
    printf("q");
}
