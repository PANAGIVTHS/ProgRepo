
// Just do int *array 4Head

#include <stdio.h>

typedef struct {
    int *p_index;
} pointer;

int main(int argc, char *argv[]) {
    int m = 1,m1 = 2,m2 = 3, m3 =4, m4=5;
    int *p1;
    int *p2;
    int *p3;
    int *p4;
    int *p5;
    pointer array2[10];  // Useless but works !!. Nope. Does not.

    p1 = &m;
    p2 = &m1;
    p3 = &m2;
    p4 = &m3;
    p5 = &m4;

    array2[1].p_index = p1;
    array2[2].p_index = p2;
    array2[3].p_index = p3;
    array2[4].p_index = p4;
    array2[7].p_index = p5;

    printf("%x|%x\n", p1, array2[-1].p_index); 
    printf("%x|%x\n", array2[3].p_index, array2[1].p_index); 
    printf("%d\n", *array2[1].p_index); 
    printf("%d\n", *(array2[2].p_index));
    printf("%d\n", *(array2[3].p_index));
    printf("%d\n", *(array2[4].p_index)); 
    printf("%d\n", *(array2[7].p_index)); 

    return(0);
}