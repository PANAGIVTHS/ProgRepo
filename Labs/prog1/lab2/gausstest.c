
//Bitmasking 4 characters to see if the first two are = to the last two
// NOTICE! when thinking arrays start from index 1 the programme handles the conversion

#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) {

      //need to cehck if i,j <= sqrt(indexNum)
    int indexNum = 16;                                                          
    
    //float twodArray[0]; //= {1,4,5,7,-2,8,-1,4,1,6,4,2,-3,0,5,-4}; //indexNum
    int i=1, k=1, o=0, z=0, bValue=0, isAugmented=0;
    float p=1;

    printf("Please insert number of indexes: \n");
    scanf("%d", &indexNum);
    printf("Is your matrix an Augmented matrix? (0=no, 1=yes)\n");
    scanf("%d",&isAugmented);

    if(isAugmented){
        indexNum += (int)sqrt(indexNum);
        bValue = 1;
    }
    
    float twodArray[indexNum];
    
    printf("Please insert value array: \n");              
    for(int f = 0; f != indexNum; f++){
        printf("index %d : ", f);
        scanf("%f", &twodArray[f]);
    }

    int arrayPos_listPosConverter(int iPos, int jPos){
        int listPos = ((iPos-1)*(((int)sqrt(indexNum))+bValue) + jPos) - 1;
        return(listPos);
    }


    for(int o = 0; o <= (indexNum - 1); o++) {
        
        ((o+1)%((int)sqrt(indexNum)+bValue) == 0) && (o != 0)? printf("%f \n", twodArray[o]) : printf("%f ", twodArray[o]);
    }
    
        for(; k <= (int)sqrt(indexNum); k++) {
            for(i=1; k + i <= (int)sqrt(indexNum); i++) {
                for(z=0; k+z <= (int)sqrt(indexNum) + bValue; z++) {                 
                    printf("a(%d,%d)[%f] test ", k+i, k,twodArray[arrayPos_listPosConverter(k+i,k)]);
                    printf("a(%d,%d)[%f] test \n", i+k, k+z,twodArray[arrayPos_listPosConverter(i+k,k+z)]);
                    printf("a(%d,%d)[%f] = ", i+k, k+z,twodArray[arrayPos_listPosConverter(i+k,k+z)]);
                    printf("a(%d,%d)[%f] - ", i+k, k+z,twodArray[arrayPos_listPosConverter(i+k,k+z)]);
                    if(twodArray[arrayPos_listPosConverter(k,k)] == 0) {continue;}
                    if(arrayPos_listPosConverter(i+k,k+z) == arrayPos_listPosConverter(k+i,k)) {
                        p = ((float)twodArray[arrayPos_listPosConverter(i+k,k)]/(float)twodArray[arrayPos_listPosConverter(k,k)]);
                        printf("yes ");
                    }
                    twodArray[arrayPos_listPosConverter(i+k,k+z)] = twodArray[arrayPos_listPosConverter(i+k,k+z)] - p
                      *twodArray[arrayPos_listPosConverter(k,k+z)];          
                    printf("( a(%d,%d) / ", i+k, k);
                    printf("a(%d,%d)[%f] ) * ", k, k,p);
                    printf("a(%d,%d)[%f]", k, k+z,twodArray[arrayPos_listPosConverter(k,k+z)]);
                    printf("   [%f]   \n", twodArray[arrayPos_listPosConverter(i+k,k+z)]);
                }
            }
        }
    
    printf("\n%d %d %d\n\n",i,k,k);
    printf("Matrix after Elimination:\n");
    for(o = 0; o <= (indexNum - 1); o++) {
        
        ((o+1)%((int)sqrt(indexNum)+bValue) == 0) && (o != 0)? printf("%.2f \n", twodArray[o]) : printf("%.2f ", twodArray[o]);
    }

    //printf("\n");
    //scanf("%d %d", &iPosition, &jPosition);
    //indexinlist = arrayPos_listPosConverter(iPosition, jPosition);
    //printf("Index in list #%d\n", indexinlist);

    return(0);
}