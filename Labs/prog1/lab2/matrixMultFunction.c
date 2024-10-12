
//Matrix multiplication

#include <stdio.h>
#include <math.h>

double* matrixMulti(double arrayProduct[], double arrayA[], int arrayAIndexes, int arrayA_columns, double arrayB[], int arrayBIndexes, int arrayB_columns) {

    int o=0, i=1 , j=1, k=1;

    int arrayPos_listPosConverter_Multi(int iPos, int jPos, int arrayColumns){
        int listPos = ((iPos-1)*((arrayColumns)) + jPos - 1);      //arrayPos to listPos converter Function
        return(listPos);
    }

    //for (o = 0; o <= (arrayAIndexes - 1); o++) {
    //    ((o+1)%((arrayA_columns)) == 0) && (o != 0)? printf("%.2f \n", arrayA[o]) : printf("%.2f ", arrayA[o]);         //show MatrixA
    //}
    //printf("\n");
    //for (o = 0; o <= (arrayBIndexes - 1); o++) {    
    //    ((o+1)%((arrayB_columns)) == 0) && (o != 0)? printf("%.2f \n", arrayB[o]) : printf("%.2f ", arrayB[o]);         //show MatrixB
    //}
    //
    
    for (int z=0; z<(arrayAIndexes/arrayA_columns)*arrayB_columns; z++) {
        arrayProduct[z] = 0;                        // all cells are 0
    }
    
    if (arrayA_columns == (arrayBIndexes/arrayB_columns)) {      // checks if array A columns == array B rows
        for (; i <= arrayAIndexes/arrayA_columns; i++) {
            for (k=1; k <= arrayB_columns;  k++) {
                for (j=1; j<= arrayA_columns; j++) {
                    arrayProduct[arrayPos_listPosConverter_Multi(i,k,arrayB_columns)] = arrayProduct[arrayPos_listPosConverter_Multi(i,k,arrayB_columns)] + arrayA[arrayPos_listPosConverter_Multi(i,j,arrayA_columns)]
                      *arrayB[arrayPos_listPosConverter_Multi(j,k,arrayB_columns)];
                    //printf("i=%d j=%d k=%d  ",i,j,k);
                    //printf("P(a[%d,%d] [%f]) += ",i,k,arrayProduct[arrayPos_listPosConverter_Multi(i,k,arrayB_columns)]);
                    //printf("A(a[%d,%d] [%f][%d]) * ",i,j,arrayA[arrayPos_listPosConverter_Multi(i,j,arrayA_columns)],arrayPos_listPosConverter_Multi(i,j,arrayA_columns));
                    //printf("B(a[%d,%d] [%f][%d])\n",j,k,arrayB[arrayPos_listPosConverter_Multi(j,k,arrayB_columns)],arrayPos_listPosConverter_Multi(j,k,arrayB_columns));

                }
            }
        }
        //for (o = 0; o <= ((arrayAIndexes/arrayA_columns)*arrayB_columns - 1); o++) {            //(arrayAIndexes/arrayA_columns)*arrayA_columns = arrayProdIndexes, array B_col = array Prod_Col
        //   if(arrayB_columns == 1) {
        //       printf("%.2f \n", arrayProduct[o]);
        //       continue;
        //   }
        //   ((o+1)%((arrayB_columns)) == 0) && (o != 0)? printf("%.2f \n", arrayProduct[o]) : printf("%.2f ", arrayProduct[o]);         //show MatrixB
        //}
    }
    
    return(arrayProduct);
}

int main() {
    double *A[1000];
    double a[4];
    double b[4] = {1,34,5},c[4] = {1,6,4,2};
    matrixMulti(b,b,4,2,c,4,2);
    printf("%lf",b[1]);
    printf("%lf",b[3]);
    printf("%lf",b[2]);
}