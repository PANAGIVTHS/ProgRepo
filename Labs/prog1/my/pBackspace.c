
#include <stdio.h>
#include <string.h>


void particlePos (char array[11][11], int index) {
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            array[i][j] = '\0';
        }
    }
    array[index/10][index%10] = '@';    
}

void changeIndex (char array[11][11], int index) {
    if (index != 0) {
        array[(index-1)/10][(index-1)%10] = ' ';
    }
    array[index/10][index%10] = '|';
}

void findLetter (char array[11][11], char c) {
    long unsigned int charTranslation[10];
    int mask = 0x1;

    switch (c) {
        case 'h':
            charTranslation[0] = 0x0;
            charTranslation[1] = 0x38e;
            charTranslation[2] = 0x184;
            charTranslation[3] = 0x184;
            charTranslation[4] = 0x184;
            charTranslation[5] = 0x1fc;
            charTranslation[6] = 0x184;
            charTranslation[7] = 0x184;
            charTranslation[8] = 0x184;
            charTranslation[9] = 0x386;
            break;
        //case 'e':
        //    charTranslation = ;
        //    break;
        //case 'l':
        //    charTranslation = ;
        //    break;
        //case 'o':
        //    charTranslation = ;
        //    break;
        //case 'w':
        //    charTranslation = ;
        //    break;
        //case 'r':
        //    charTranslation = ;
        //    break;
        //case 'd':
        //    charTranslation = ;
        //    break;
        //case '!':
        //    charTranslation = ;
        //    break;
    }
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            array[i][j] = (((charTranslation[i] >> j) & mask) == 0? (' ') : ('#'));
        }
    }
}

int main(int argc, char *argv[]) {
    char enter;
    char array[11][11] = {0};
    /*
    findLetter(array, 'h');
    for(int i = 0; i < 100; i++) {
        changeIndex(array, i);
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k <10; k++) {
                printf("%c ", array[j][k]);
            }
            printf("\n");
        }
        scanf("%c", &enter);
        printf("%c[2J%c[;H",(char) 27, (char) 27);
    } 
    */
    int values[9] = {90, 41, 22, 34, 65, 96, 87, 88, 99};

    for(int i = 0; i < 100; i++) {
        particlePos(array, values[i%9]);
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k <10; k++) {
                printf("%c  ", array[j][k]);
            }
            printf("\n");
        }
        scanf("%c", &enter);
        printf("%c[2J%c[;H",(char) 27, (char) 27);
    }
}   
