
//Sport ticket decoder. Takes a Hex number and decrypts it, extracting info like: match time, seat, gate, date, row.

#include <stdio.h>
#define timeBits 5
#define dateBits 5
#define monthBits 4
#define seatBits 7
#define rowBits 6
#define gateBits 5

int main(int argc, char *argv[]) {

    unsigned int codeInput=0, startTime=0, dateDays=0, dateMonths=0, seatValue=0, gateValue=0, rowValue=0;
    unsigned int shiftLeftValue = timeBits, chooseCase=0, currentMonth=0, currentDay=0, currentTime=0;
    char section;

    printf("Enter code:\n");
    scanf("%x", &codeInput);
    
    startTime = (codeInput << (32 - shiftLeftValue)) >> (32 - timeBits);
    dateDays = (codeInput << (32 - (shiftLeftValue += dateBits))) >> (32 - dateBits);
    dateMonths = (codeInput << (32 - (shiftLeftValue += monthBits))) >> (32 - monthBits);       //Shifts potion and gives wanted value
    seatValue = (codeInput << (32 - (shiftLeftValue += seatBits))) >> (32 - seatBits);
    gateValue = (codeInput << (32 - (shiftLeftValue += gateBits))) >> (32 - gateBits);
    rowValue = (codeInput) >> (32 - rowBits);

    printf("%02u/%02u %02u:00 %u %c %u\n", dateDays, dateMonths, startTime, rowValue, 'A' + gateValue, seatValue);
    
    chooseCase = (seatValue - 1)/40;    // -1 for the division to work. instead of 1-40 divides by 0-39 (to opoio dinei to epi8umhto phliko)
    
    switch(chooseCase) {
        case 0:
            section = 'A';
            break;
        case 1:
            section = 'B';  //finds section for any given seat Number
            break;
        default:
            section = 'C';
            break;
    }
    printf("Section %c\n", section);
    printf("\n##\n");
    printf("Now:\n");
    scanf("%u/%u,%u", &currentDay, &currentMonth, &currentTime);

    if((currentMonth > dateMonths) ||  ((currentDay>dateDays) && (currentMonth == dateMonths)) || ((currentMonth == dateMonths) && (currentDay == dateDays) && (currentTime > startTime))) {
        printf("Missed game.\n");
    }

    return(0);
}