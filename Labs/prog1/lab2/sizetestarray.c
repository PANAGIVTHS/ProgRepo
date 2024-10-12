
//Bitmasking 4 characters to see if the first two are = to the last two
// NOTICE! when thinking arrays start from index 1 the programme handles the conversion

#include <stdio.h>
#include <math.h>

// Creates an array of all intigers in a given P array (these intigers are in binary form across rows)
// converts these numbers to P array with any changes in order by user

int createP(double arrayP[], int linesOrCol, int indexNum, int line_col_Number1, int line_col_Number2) {
    
    int tempValue;
    int arrayPlinesCol[(int)sqrt(indexNum)];
    short int mask = 0x1;

    if (linesOrCol) {
        for (int i = (int)sqrt(indexNum)-1, k = 0; i >= 0; i--, k++) {
            arrayPlinesCol[k] = (mask << i);
            //printf("%d,%d ",(mask << i), i);
        }
        tempValue = arrayPlinesCol[line_col_Number1 - 1];
        arrayPlinesCol[line_col_Number1 - 1] = arrayPlinesCol[line_col_Number2 - 1];
        arrayPlinesCol[line_col_Number2 - 1] = tempValue;

        //for (int o = 0; o < 4; o++) {
        //    printf("%d\n", arrayPlinesCol[o]);
        //}

        for (int k=0; k < (int)sqrt(indexNum); k++) {
            for (int i = 0 , j = (int)sqrt(indexNum) -1; i < (int)sqrt(indexNum);  j--, i++) {
                (arrayPlinesCol[k] & (mask << j)) == 0 ? (arrayP[i + k*(int)sqrt(indexNum)] = 0) : (arrayP[i + k*(int)sqrt(indexNum)] = 1);
                //printf(" %d.%x[%d]",arrayPlinesCol[k] , arrayPlinesCol[k] & (mask << j), (mask << j));
                //printf("%d%d%d ",i,j,k);
            }
        }
    }
    else {}     //enalagh column

    //printf("\n");
    //for (int o = 0; o < indexNum; o++) {
    //    ((o+1)%(((int)sqrt(indexNum))) == 0) && (o != 0)? printf("%lf %d\n", arrayP[o],o) : printf("%lf ", arrayP[o]);         //show MatrixA
    //}
    return 0;
}

int main() {
    double ap[16];
    int indexNum = 16;

    createP(ap, 1, 16, 1,4);

    for (int o = 0; o < indexNum; o++) {
        ((o+1)%(((int)sqrt(indexNum))) == 0) && (o != 0)? printf("%lf\n", ap[o]) : printf("%lf ", ap[o]);         //show MatrixA
    }
}