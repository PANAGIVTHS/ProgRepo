
#include"lab9.h"
#include <stdio.h>
#include <math.h>

#define AMOEBA_DISTANCE 0.5

enum status {MERGED, DIED, ADDED, ERROR};
enum values {UNDA, SAPPINIA, CHAOS};
enum isValid {VALID, INVALID};

typedef struct position {
    double x;
    double y;
} position_t;


typedef struct amoebaInfo {
    enum values genus;
    position_t petriDishPos;
    enum isValid valid;
    double amoebaSize;
} amoebaInfo_t;

void intialiseArray (amoebaInfo_t *amoebaArray, const int dish_size) {
    for (int i = 0; i < dish_size; i++) {
        amoebaArray[i].valid = INVALID;
        amoebaArray[i].genus = CHAOS;
        amoebaArray[i].amoebaSize = 0;
        amoebaArray[i].petriDishPos.x = 0;
        amoebaArray[i].petriDishPos.y = 0;
    }
}

void printAmoeba (amoebaInfo_t *amoebaArray, const int dish_size) {
    for (int i = 0; i < dish_size; i++) {
        if (amoebaArray[i].valid == VALID) {
            printf("%d: (%4.1lf, %4.1lf) %4.1lf", i, amoebaArray[i].petriDishPos.x, amoebaArray[i].petriDishPos.y, amoebaArray[i].amoebaSize);
            switch (amoebaArray[i].genus)
            {
            case UNDA: {
                printf(" UNDA\n");
                break;
            }
            case SAPPINIA: {
                printf(" SAPPINIA\n");
                break;
            }
            case CHAOS: {
                printf(" CHAOS\n");
                break;
            }
            default:
                break;
            }
        }
    }
}

int scanInfo (amoebaInfo_t *amoebaPtr) {
    int genusTypeTemp;
    double sizeTemp;
    
    printf("Enter genus: 0=UNDA 1=SAPPINIA 2=CHAOS\n");
    scanf("%d", &genusTypeTemp);
    if (genusTypeTemp < 0 || genusTypeTemp > 2) {
        return(-1);
    }
    amoebaPtr->genus = genusTypeTemp;
    printf("Enter posotion: x y\n");
    scanf("%lf %lf", &(amoebaPtr->petriDishPos.x), &(amoebaPtr->petriDishPos.y));
    printf("Enter size:\n");
    scanf("%lf", &sizeTemp);
    if (sizeTemp <= 0) {
        return(-1);
    }
    amoebaPtr->amoebaSize = sizeTemp;
    return(0);
}

double pointDistance (double x1, double y1, double x2, double y2) {
    return(sqrt(pow(x2 - x1,2) + pow(y2 - y1,2)));
}

enum status setPosition (amoebaInfo_t *amoebaArray, amoebaInfo_t *indAmoeba, const int SIZE_OF_DISH) {
    int x1,y1;
    int firstInvalid = -1;
    int numOfValids=0;

    for (int i = 0; i < SIZE_OF_DISH; i++) {
        if (amoebaArray[i].valid == INVALID) {
            amoebaArray[0] = *indAmoeba;
            return(ADDED);
        }
        if (amoebaArray[i].valid == VALID) {
            y1 = amoebaArray[i].petriDishPos.y;
            x1 = amoebaArray[i].petriDishPos.x;
            if (pointDistance(x1, y1, indAmoeba->petriDishPos.x, indAmoeba->petriDishPos.y) < AMOEBA_DISTANCE) {
                // found in range
                if (amoebaArray[i].genus == indAmoeba->genus) {
                    amoebaArray[i].amoebaSize += indAmoeba->amoebaSize;
                    return(MERGED);
                }
                else {
                    amoebaArray[i].valid = INVALID;
                    return(DIED);
                }
            }
        }
        else {
            if (firstInvalid == -1) {
                firstInvalid = i;
            }
            if ((i == SIZE_OF_DISH-1) && (firstInvalid != -1)) {
                amoebaArray[firstInvalid] = *indAmoeba;
                return(ADDED);
            } 
            else {
                return(ERROR);
            }
        }
    }
    return(ERROR);
}

int main (int argc, char *argv[]) {
    amoebaInfo_t amoebaArray[DISH_SIZE];
    amoebaInfo_t indAmoeba;

    char loopSelector;
    int amoebaCounter = 0;
    intialiseArray(&amoebaArray[amoebaCounter], DISH_SIZE);

    do {
        printf("More? (y/n)\n");
        scanf(" %c", &loopSelector);

        if (loopSelector == 'n') {
            return(1);
        }

        if(scanInfo(&indAmoeba) != -1) {
            switch (setPosition(amoebaArray, &indAmoeba, DISH_SIZE)) {
                case MERGED: {
                    printf("Merged.\n");
                    break;
                }
                case DIED: {
                    printf("Cross-canceled.\n");
                    amoebaCounter--;
                    break;
                }
                case ADDED: {
                    printf("Added.\n");
                    amoebaArray[amoebaCounter].valid = VALID;
                    amoebaCounter++;
                    break;
                }
                default: {
                    printf("No space.\n");
                    break;
                }
            }

        }
        else {
            printf("Invalid data.\n");
            continue;
        }
        printAmoeba(amoebaArray, DISH_SIZE);
    } while(amoebaCounter < DISH_SIZE);
    

	return(0);
}