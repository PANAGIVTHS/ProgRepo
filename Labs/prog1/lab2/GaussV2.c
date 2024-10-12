

// NOTICE! when thinking arrays start from index 1 the programme handles the conversion

#include <stdio.h>
#include <math.h>

int createP(double arrayP[], int indexNum, int line_col_Number1, int line_col_Number2) {
    
    int tempValue;
    int arrayPlinesCol[(int)sqrt(indexNum)];
    short int mask = 0x1;

        for (int i = (int)sqrt(indexNum)-1, k = 0; i >= 0; i--, k++) {
            arrayPlinesCol[k] = (mask << i);
        }

        tempValue = arrayPlinesCol[line_col_Number1 - 1];
        arrayPlinesCol[line_col_Number1 - 1] = arrayPlinesCol[line_col_Number2 - 1];
        arrayPlinesCol[line_col_Number2 - 1] = tempValue;

        for (int k=0; k < (int)sqrt(indexNum); k++) {
            for (int i = 0 , j = (int)sqrt(indexNum) -1; i < (int)sqrt(indexNum);  j--, i++) {
                (arrayPlinesCol[k] & (mask << j)) == 0 ? (arrayP[i + k*(int)sqrt(indexNum)] = 0) : (arrayP[i + k*(int)sqrt(indexNum)] = 1);
            }
        }
    return 0;
}

double* matrixMulti(double arrayProduct[], double arrayA[], int arrayAIndexes, int arrayA_columns, double arrayB[], int arrayBIndexes, int arrayB_columns) {

    int i=1 , j=1, k=1;

    int arrayPos_listPosConverter_Multi(int iPos, int jPos, int arrayColumns){
        int listPos = ((iPos-1)*((arrayColumns)) + jPos - 1);      //arrayPos to listPos converter Function
        return(listPos);
    }
    
    for (int z=0; z<(arrayAIndexes/arrayA_columns)*arrayB_columns; z++) {
        arrayProduct[z] = 0;     // all cells are 0 cant do arrayProduct = {0} because index isnt const
    }
    
    if (arrayA_columns == (arrayBIndexes/arrayB_columns)) {      // checks if array A columns == array B rows
        for (; i <= arrayAIndexes/arrayA_columns; i++) {
            for (k=1; k <= arrayB_columns;  k++) {
                for (j=1; j<= arrayA_columns; j++) {
                    arrayProduct[arrayPos_listPosConverter_Multi(i,k,arrayB_columns)] = arrayProduct[arrayPos_listPosConverter_Multi(i,k,arrayB_columns)] + arrayA[arrayPos_listPosConverter_Multi(i,j,arrayA_columns)]
                      *arrayB[arrayPos_listPosConverter_Multi(j,k,arrayB_columns)];
                }
            }
        }
    }
    
    return(arrayProduct);
}

int arrayPos_listPosConverter(int iPos, int jPos, int index_Number,int augment){
    int listPos = ((iPos-1)*(((int)sqrt(index_Number))+augment) + jPos) - 1;      //arrayPos to listPos converter Function
    return(listPos);
}

double gaussElim(double arrayForElim[], int isAugmented,int size, double swapArray[]) {

    int i=1, k=1,/* o=0,*/ z=0;
    float p=1;

    int arrayForElimIndexes = size + isAugmented*((int)sqrt(size));
        for(; k <= (int)sqrt(size); k++) {
            for(i=1; k + i <= (int)sqrt(size); i++) {
                for(z=0; k+z <= (int)sqrt(size)+isAugmented; z++) {                 
                    if(arrayForElim[arrayPos_listPosConverter(k,k,size,isAugmented)] == 0) {
                        for(int v=1; (v <= (int)sqrt(size) - k) || arrayForElim[arrayPos_listPosConverter(k+v,k, size, isAugmented)] != 0; v++) {
                            //find P
                            createP(swapArray, size, k, v);
                            //create temporary array
                            double multiTempArray[arrayForElimIndexes];
                            matrixMulti(multiTempArray, swapArray, size, (int)sqrt(size), arrayForElim, arrayForElimIndexes, (int)sqrt(size)+isAugmented);
                            for (int p = 0; p < arrayForElimIndexes; p++) {
                                arrayForElim[p] = multiTempArray[p];
                            }
                        }
                        continue;
                    }
                    if(arrayPos_listPosConverter(i+k,k+z,size,isAugmented) == arrayPos_listPosConverter(k+i,k, size, isAugmented)) {
                        p = (arrayForElim[arrayPos_listPosConverter(i+k,k, size, isAugmented)]/arrayForElim[arrayPos_listPosConverter(k,k,size,isAugmented)]);        //Gauss Algorithm
                    }
                    arrayForElim[arrayPos_listPosConverter(i+k,k+z,size,isAugmented)] = arrayForElim[arrayPos_listPosConverter(i+k,k+z,size,isAugmented)] - p
                      *arrayForElim[arrayPos_listPosConverter(k,k+z, size, isAugmented)];          
                }
            }
        }
    return(0);
}


int main() { 
    double array[16] = {0, 0, 1,1,0,3,2,1,5,1,2,5,62,1,3};
    double pArray[16];
    gaussElim(array, 0, 16, pArray);
    
    for(int i = 0; i<16; i++) {
        if(i%4 == 0) {printf("\n");}
        printf("%6.2lf ", array[i]);
    }
    printf("\n\n");
    return 0;
}