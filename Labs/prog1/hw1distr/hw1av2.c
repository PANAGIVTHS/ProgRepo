/*
 * ONOMATEPWNYMO:
 * AEM:
 */
#include<stdio.h>
#include"hw1.h"

int main (int argc, char *argv[]) {
	unsigned int maxChoice;
	//3rd dimension in array stores whether the answer is correct (1)
	char arrayAnswerKey[NUM_QUESTIONS], arrayStudentAnswers[NUM_STUDENTS][NUM_QUESTIONS + 1][2]; 
	int iCounter = 0;
	float answerPenalty;

	printf("Enter number of choices:\n");
	do {
		scanf("%u", &maxChoice);
	} while (maxChoice > 27 && maxChoice < 0);
	printf("Max choice: '%c'\n", maxChoice - 1 + 'A');
	printf("Enter answer key:\n");
	
	for (iCounter = 0; iCounter < NUM_QUESTIONS; iCounter++) {
		scanf(" %c", &arrayAnswerKey[iCounter]);
		if (arrayAnswerKey[iCounter] - 'A' > maxChoice - 1) {
			do {
				printf("Error. Enter value A-%c\n", maxChoice - 1 + 'A');
				scanf(" %c", &arrayAnswerKey[iCounter]);
			} while (arrayAnswerKey[iCounter] - 'A' > maxChoice - 1);
		}
	}
	printf("Enter wrong answer penalty:\n");
	do {
		scanf("%f", &answerPenalty);
		if (answerPenalty > 0) {
			printf("Error. Enter non-positive value:\n");
		} 
	} while (answerPenalty > 0);

	for (int studentCounter = 0; studentCounter < NUM_STUDENTS; studentCounter++) {	
		printf("Enter student answers:\n");
		// write k-student's answers (k = studentCounter)
		for(int questionCounter = 0; questionCounter < NUM_QUESTIONS; questionCounter++) {
			scanf(" %c", &arrayStudentAnswers[questionCounter][studentCounter][0]);
			if (arrayStudentAnswers[questionCounter][studentCounter][0] - 'A' > maxChoice - 1) {
				do {
					printf("Error. Enter value A-%c\n", maxChoice - 1 + 'A');
					scanf(" %c", &arrayStudentAnswers[questionCounter][studentCounter][0]);
				} while (arrayStudentAnswers[questionCounter][studentCounter][0] - 'A' > maxChoice - 1);
			}
			if (arrayStudentAnswers[questionCounter][studentCounter][0] == arrayAnswerKey[questionCounter]) {
                arrayStudentAnswers[questionCounter][studentCounter][1] = 
            }
		}
			// write k-student's grade
			max(0, )
	}

	return 0;
}
