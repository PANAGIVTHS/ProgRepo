
//Bitmasking 4 characters to see if the first two are = to the last two

#include <stdio.h>

int main(int argc, char *argv[]) {

    unsigned int hexDigit_input, lastDigits, firstDigits;
    int mask = 0xff, mask2 = 0xff00;

    printf("Enter 4 hex digits: ");
    scanf("%x", &hexDigit_input);           
    firstDigits = ((hexDigit_input & mask2)>>8);    //takes the first two digits
    lastDigits = hexDigit_input & mask;             //takes last 2 digits
    
    (lastDigits == firstDigits)?printf("yes\n"):printf("no\n"); 

    return(0);
}