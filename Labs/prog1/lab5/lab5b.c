
// Programme to check if matrix is symmetrical

#include <stdio.h>
#include"lab5.h"
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int comparisonNum = 0;
    int matrix[MATRIXSIZE][MATRIXSIZE];
    int number, breakPoint = 0;

    // Set matrix with input values
    for (int i = 0; i < MATRIXSIZE; i++) {
        for (int j = 0; j < MATRIXSIZE; j++) {
            scanf("%d", &number);
            matrix[i][j] = number;
        }
    }
    // Check if the matrix is symmetrical
    for (int i = 0; i < MATRIXSIZE; i++) {
        for (int j = 0; j < MATRIXSIZE; j++) {      // Could also store the values in this loop
            if (i-j >= 1) {                         // and do less loops below (line: 30 - ..)
                comparisonNum++;                    // more complex, prob faster
                if (matrix[i][j] != matrix[j][i]) {
                    breakPoint = 1;
                }
            }
        }
    }
    // braekPoint = 1 | not symmetrical. 
    if (breakPoint) {
        printf("NO\n");
        for (int i = 0; i < MATRIXSIZE; i++) {
            for (int j = 0; j < MATRIXSIZE; j++) {
                printf("%3d ", matrix[i][j]);
            }
            printf("\n");
        }
    }
    else {
        // Print all terms below the diagonal (i = j)
        printf("YES\n");
        for (int i = 0; i < MATRIXSIZE; i++) {
            for (int j = 0; j < MATRIXSIZE; j++) {
                if (i - j >= 1) printf("%3d ", matrix[i][j]); 
            }
            printf("\n");
        }
    }
    fprintf(stderr, "%d\n\n", comparisonNum);
    
    return(0);
}