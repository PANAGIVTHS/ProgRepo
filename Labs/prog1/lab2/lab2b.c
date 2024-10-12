
//Programme: Inches to Yards/Feet/inches converter 
// Takes inches as input and spits out yards, feet, inches

#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) {

    unsigned int inchesInput, yardsValue, inchesValue, feetValue;

    printf("Inches:\n");
    scanf("%u", &inchesInput);
    yardsValue = inchesInput / 36;                                          
    feetValue = (inchesInput - (yardsValue * 36)) / 12;                     //conversion process
    inchesValue = inchesInput - ((feetValue * 12) + (yardsValue*36));       
    
    printf("%u inches = ", inchesInput);
    (yardsValue == 0)? (printf(" ")) : (printf("%u yd ", yardsValue));      //prints values of feet,yards,inches unless they are 0
    (feetValue == 0)? (printf(" ")) : (printf("%u ft ", feetValue));
    (inchesValue == 0)? (printf(" \n")) : (printf("%u in\n", inchesValue));

    return(0);
}