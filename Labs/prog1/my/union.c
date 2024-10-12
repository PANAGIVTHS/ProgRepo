
#include <stdio.h>
#include <string.h>

typedef union arrayTypes {
    int arrayI[100];
    char arrayC[100];
} t_type;

typedef struct symbols {
    int symbolI;
    char symbolC;
} t_symbol_type;



void replaceIndex (t_type *array, size_t type, int index, t_symbol_type symbol) {
    if (type && (array->arrayC[index] != '\0')) {
        array->arrayC[index] = (symbol.symbolC);
    }
    else if(array->arrayI[index] != 0) {
        array->arrayI[index] = (symbol.symbolI);
    }
}

void printUnion (t_type *array, size_t type) {
    for (int i = 0; i < 40; i++) {
        if (type) {
            printf("%c ", array->arrayC[i]);
        }
        else {
            printf("%d ", array->arrayI[i]);
        }
    }
}

void strcpyInt (int *array, int *array2) {
    for (int i = 0; i < 40; i++) {
        array[i] = array2[i];
    }
}

int main(int argc, char *argv[]) {
    t_type array[100];
    int arrayInt[40] = {0,1,2,3,4,5,6,7,89,9,99,9,99};
    t_symbol_type input;

    input.symbolC = '@';
    input.symbolI = 0;
    strcpyInt(array->arrayI, arrayInt);

    scanf("%s", array->arrayC);
    replaceIndex(array, 1, 2, input);
    printUnion(array, 1);
    printf("\n");

    return(0);
}   
