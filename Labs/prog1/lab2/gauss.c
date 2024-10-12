
//Bitmasking 4 characters to see if the first two are = to the last two
// NOTICE! when thinking arrays start from index 1 the programme handles the conversion

#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) {

    int indexNum = 16;                                                          
    
    int i=1, k=1, o=0, z=0, bValue, isAugmented;
    float p=1;

    printf("Please insert number of indexes: \n");
    scanf("%d", &indexNum);
    printf("Is your matrix an Augmented matrix? (0=no, 1=yes)\n");      //Is Augmented?
    scanf("%d",&isAugmented);

    if(isAugmented){
        indexNum += (int)sqrt(indexNum);
        bValue = 1;
    }

    float twodArray[indexNum];
    printf("Please insert value array: \n");              //arraySpecifiers
    for(int f = 0; f != indexNum; f++){
        printf("index %d : ", f);
        scanf("%f", &twodArray[f]);
    }

    int arrayPos_listPosConverter(int iPos, int jPos){
        int listPos = ((iPos-1)*(((int)sqrt(indexNum))+bValue) + jPos) - 1;      //arrayPos to listPos converter Function
        return(listPos);
    }


    for(int o = 0; o <= (indexNum - 1); o++) {
        
        ((o+1)%((int)sqrt(indexNum)+bValue) == 0) && (o != 0)? printf("%.2f \n", twodArray[o]) : printf("%.2f ", twodArray[o]);         //show Matrix0
    }
    
        for(; k <= (int)sqrt(indexNum); k++) {
            for(i=1; k + i <= (int)sqrt(indexNum); i++) {
                for(z=0; k+z <= (int)sqrt(indexNum)+bValue; z++) {                 
                    if(twodArray[arrayPos_listPosConverter(k,k)] == 0) {continue;}
                    if(arrayPos_listPosConverter(i+k,k+z) == arrayPos_listPosConverter(k+i,k)) {
                        p = (twodArray[arrayPos_listPosConverter(i+k,k)]/twodArray[arrayPos_listPosConverter(k,k)]);        //Gauss Algorithm
                    }
                    twodArray[arrayPos_listPosConverter(i+k,k+z)] = twodArray[arrayPos_listPosConverter(i+k,k+z)] - p
                      *twodArray[arrayPos_listPosConverter(k,k+z)];          
                }
            }
        }
    
    printf("Matrix after Elimination:\n");
    for(o = 0; o <= (indexNum - 1); o++) {
        ((o+1)%((int)sqrt(indexNum)+bValue) == 0) && (o != 0)? printf("%.2f \n", twodArray[o]) : printf("%.2f ", twodArray[o]);         //show Matrix
    }

    return(0);
}