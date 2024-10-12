//lab1 exercise

#include<stdio.h>

int main(int argc, char *argv[]){
    
    int amka;
    int idNum;
    char firstChar;
    char secChar;
    char nameChar1;
    char nameChar2;
    float Euros;


    printf("Info:\n");
    scanf("AMKA:%d ID:%c%c%d",&amka ,&firstChar, &secChar, &idNum);
    printf("Name:");
    scanf(" %c%c", &nameChar1, &nameChar2);
    printf("How much:\n");
    scanf("%f", &Euros);
    printf("Patient %c.%c.(%011d, %c%c%06d)\nAmount: %.2f euros.\n", nameChar1, nameChar2, amka, firstChar, secChar, idNum, Euros);
    
    return(0);
}