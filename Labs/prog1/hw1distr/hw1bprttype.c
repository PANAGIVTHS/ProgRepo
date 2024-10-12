/* 
 * ONOMATEPWNYMO: Tsogkas Pnagiotis Nikolaos
 * AEM: 03NUM_CANDIDATES72
 */
#include"hw1.h"
#include <stdio.h>
#include <math.h>

#define INDEX_BEFORE_AGE 9
#define INDEX_BEFORE_GENDER 7
#define NUM_OF_PARAMETERS 3 
#define AGE_GROUPS 4

int main (int argc, char *argv[]) {

	int result, value, isPowerOfTwo, tempMostVotes, winner, votedByMen, votedByWomen, votedByOther;
    char menuInput, genderChar;
	unsigned short int voterCode = 0;
	unsigned short int voterDataArray[MAX_VOTERS][NUM_OF_PARAMETERS]; // Columns index 0 = age, 1 = gender, 2 = canditate
	int votesArray[INDEX_BEFORE_GENDER][4] = {0}; // Column 0 for all votes , ind 1 for male , 2 female , 3 other
    int ageRangeArray[AGE_GROUPS] = {0};
	unsigned long int mask1 = 0x7f, mask2 = 0x180, mask3 = 0xfe00 , maskNumOfCand =0x1;
	int excessVotes=0, mostVotesOverall, mostVotesMen, mostVotesWomen, mostVotesOther, ageCase;
    
    // tupou int afou jerw oti 8a parei mono powers of 2
    // log2(Canditate number) basically 
    // orismos mesa sthn main gt exei problhmata me thn math.h
    int log2(int input) {
            for (int i = 0; (i < INDEX_BEFORE_GENDER); i++) {
                // checks if the LSB is a 1
		    	if (((input >> i ) == maskNumOfCand)) { 
		    		input = i; // Or use log(n)/log(2) from math.h
                    return(i);
		    	}
		    }
        return(0);
    }

	for (int voterCounter = 0; voterCode == 0 || voterCounter < MAX_VOTERS; voterCounter++) {
		do {
			printf("Enter voter info:\n");
			scanf(" %hx", &voterCode);
			if (voterCode != 0) {
				voterDataArray[voterCounter][2] = (voterCode & mask1);
				voterDataArray[voterCounter][1] = (voterCode & mask2) >> INDEX_BEFORE_GENDER;
				voterDataArray[voterCounter][0] = (voterCode & mask3) >> INDEX_BEFORE_AGE;
                value = voterDataArray[voterCounter][2];
                // Check if vote is valid to avoid calculations for later.
                if (value != 1) {
                    while (value != 1) {
                        if (value == 2) {
                            isPowerOfTwo = 1;
                            break;
                        }
                        value /= 2;
                    	result = value % 2;
                    	if ((result != 0)|| value == 0) {
                    		isPowerOfTwo = 0;
                            break;
                    	}
                        else {
                            isPowerOfTwo = 1;
                        } 
                    }
                }
                else {
                    isPowerOfTwo = 1;
                }
			    excessVotes = 0;
			    for (int i = 0; (i < INDEX_BEFORE_GENDER); i++) {
                    // checks if the LSB is a 1
			    	if ((((voterDataArray[voterCounter][2] >> i) & maskNumOfCand) == maskNumOfCand) && (isPowerOfTwo == 0)) { 
			    		if ((voterDataArray[voterCounter][2] >> i) > 1) {	 // checks if the binary form of the canditate votes has
			    			excessVotes++;						         // less than one 1 left. Otherwise keep increasing the number of excess votes
			    		}
			    	}
			    }
			    if (voterDataArray[voterCounter][0] < 18) {
			    	printf("Invalid age %d.\n", voterDataArray[voterCounter][0]);
			    }
                else if ((voterDataArray[voterCounter][1] != 0) && ((isPowerOfTwo == 1) || voterDataArray[voterCounter][2] == 0)){
                    // How many in age group
                    ageCase = (voterDataArray[voterCounter][0] - 15) / 15;
                    if (voterDataArray[voterCounter][0] >= 60) {
                        ageCase = 3;
                    }
                    switch(ageCase) {
                        case 0: {
                            ageRangeArray[0]++;
                            break;
                        }
                        case 1: {
                            ageRangeArray[1]++;
                            break;
                        }
                        case 2: {
                            ageRangeArray[2]++;
                            break;  
                        }
                        default: {
                            ageRangeArray[3]++;
                            break;
                        }
                    }
                }
			    if (voterDataArray[voterCounter][1] == 0) {
			    	printf("Invalid gender.\n");
			    }
			    if ((voterDataArray[voterCounter][2] != 0) && (isPowerOfTwo == 0)) {
			    	printf("Invalid votes %d.\n", excessVotes + 1);
			    }
                if (voterDataArray[voterCounter][2] == 0) {
                    voterDataArray[voterCounter][2] = 0;
                }
                if (isPowerOfTwo) {
                    //printf("%d",log2(voterDataArray[voterCounter][2]) );
                    votesArray[log2(voterDataArray[voterCounter][2])][voterDataArray[voterCounter][1]]++; // Add vote based on gender. voterDataArray[voterCounter][1] values can be 1,2,3
                    votesArray[log2(voterDataArray[voterCounter][2])][0]++; // Add to overall votes
                }
            }
		} while ((((isPowerOfTwo == 0) && (voterDataArray[voterCounter][2] != 0)) || (voterDataArray[voterCounter][1] == 0) || (voterDataArray[voterCounter][0] < 18)) && voterCode != 0);
        
        if (voterCode == 0) {
			voterDataArray[voterCounter][0] = 0;
			voterDataArray[voterCounter][1] = 0;
			voterDataArray[voterCounter][2] = 0;
			excessVotes = 0;
			break;
		}
	}
	// Find most voted per category. 1st k-loop (overall) 2nd (men) 3rd (women) 4th (other)
    for (int k = 0; k < 4; k++) {
        // Find most votes per category and find which canditate has them.
        tempMostVotes = fmax(votesArray[0][k], fmax (votesArray[1][k], fmax (votesArray[2][k], fmax(votesArray[3][k] , fmax(votesArray[4][k],votesArray[5][k])))));
        //printf("%d :", tempMostVotes);
        for (int j = NUM_CANDIDATES; j >= 0; j--) {
            //printf("j,k[%d,%d] (%d)\n", j, k, votesArray[j][k]);
            if (tempMostVotes == votesArray[j][k]) {
                switch (k) {
                    case 0: {
                        //printf("test0\n");
                        mostVotesOverall = votesArray[j][k];
                        winner = j;
                        break;
                    }
                    case 1: {
                        //printf("test1\n");
                        mostVotesMen = votesArray[j][k];    
                        votedByMen = j;
                        break;
                    }
                    case 2: {
                        //printf("test2\n");
                        mostVotesWomen = votesArray[j][k];
                        votedByWomen = j;
                        break;
                    }
                    case 3: {
                        //printf("test3\n");
                        mostVotesOther = votesArray[j][k];
                        votedByOther = j;
                        break;
                    }
                } 
            }
        }
    } 
    for (int voterCounter = 0; voterCounter < MAX_VOTERS; voterCounter++) {
        if (voterDataArray[voterCounter][2] != 0) {
            voterDataArray[voterCounter][2] = log2(voterDataArray[voterCounter][2]) + 1; // +1 to differenciate canditate #0 and non-vote 0
            //printf(" %d\n", voterDataArray[voterCounter][2]);
        }
    }
    do {
        // Prints menu
        printf("\n[P/p] Print voter info\n[C/c] Print candidate info\n[W/w] Print winner\n[G/g] Print choice by gender\n[A/a] Print ages\n[Q/q] Quit\n\n");
        scanf(" %c", &menuInput);
        switch(menuInput) {
			case 'p': 
			case 'P': {
				//Print voter info
				printf("\n##\n");
                if (voterCode != 0) {
				    for (int i = 0; i < MAX_VOTERS; i++) {
                        if (voterDataArray[i][1] == 1) {
                            genderChar = 'M';
                        } else if (voterDataArray[i][1] == 2) {
                            genderChar = 'F';
                        } else {
                            genderChar = 'O';
                        }
                        // printf("%d", voterDataArray[i][1]);
                        // Checks if the voter voted for someone
                        printf("%3d: %3d %c", i, voterDataArray[i][0], genderChar);
                        voterDataArray[i][2] == 0? printf(" -\n") : printf(" %d\n", voterDataArray[i][2] - 1);
                    }
                }
                break;
			}
			case 'c':
			case 'C': {
                // Print canditate info
                printf("\n##\n");
                for (int i = 0; i < NUM_CANDIDATES; i++) {
                    printf("%3d: %3d %3d %3d\n", i, votesArray[i][1], votesArray[i][2], votesArray[i][3]);
                }
                break;
			}
			case 'g':
			case 'G': {
                printf("\n##\n");
                printf("M: %d (%d)\n", votedByMen, mostVotesMen);
                printf("F: %d (%d)\n", votedByWomen, mostVotesWomen);
                printf("O: %d (%d)\n", votedByOther, mostVotesOther);
                break;
			}
			case 'w':
			case 'W': {
                printf("\n##\n");
                printf("%d: %d\n", winner, mostVotesOverall);
                break;
			}
			case 'q':
			case 'Q': {
				//Quit programme
				return (0);
			}
			case 'a':
			case 'A':{
                printf("\n##\n");
                printf("[18-29]: %d\n", ageRangeArray[0]);
                printf("[30-44]: %d\n", ageRangeArray[1]);
                printf("[45-59]: %d\n", ageRangeArray[2]);
                printf("[60-127]: %d\n", ageRangeArray[3]);
                break;
            }
            default: {
                printf("Error. Invalid option.\n");
            }
		}
    } while (1);
}
