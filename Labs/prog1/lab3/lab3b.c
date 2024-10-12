
//Guess the Number game w/ randomly generated Number from 0-100 

#include<stdio.h>
#include<stdlib.h>

#define LOWER_BOUND 1
#define UPPER_BOUND 100

int main (int argc, char *argv[]) {

    unsigned int seed, target;
    int triesNumber, guess;

    printf("Enter seed:\n");
    scanf("%u", &seed);
    srand(seed);
    target = rand()%(UPPER_BOUND-LOWER_BOUND+1)+LOWER_BOUND;

    do {
        printf("Enter tries:\n");
        scanf("%d", &triesNumber);
    } while ((triesNumber < 1) || (triesNumber > 10));
    if(triesNumber == 0) {
        printf("You have %d tries.\n", triesNumber);        //Saves number of tries if 0 < tries < 11
    }
    else {
        printf("You have %d try.\n", triesNumber);
    }
    printf("\n##\n");

    do {
        printf("Guess:\n");
        scanf("%d", &guess);
        if(guess == target) {
            printf("You won!\n");
            break;
        }
        else if (target > guess){
            printf("Try higher.\n");           //Checks if out of tries and/or Won the game
        }
        else {
            printf("Try lower.\n");
        }
        triesNumber--;
        printf("Tries left: %d\n", triesNumber);
    } while(triesNumber > 0);

    if (triesNumber == 0) {
        printf("You lost!\n");
    }
    printf("The number was %u.\n", target);

    return 0;
}
