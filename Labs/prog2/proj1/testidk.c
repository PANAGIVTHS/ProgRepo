
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main (int argc, char* argv[]) {
    unsigned long int i = 0;

    for (i = 1; i <= atoi(argv[1]); i+=2) {
        printf("f %lu\n", i);
    }
}
