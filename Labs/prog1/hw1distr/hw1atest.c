/*
 * ONOMATEPWNYMO:
 * AEM:
 */
#include<stdio.h>
#include"hw1.h"
#include<math.h>

int main (int argc, char *argv[]) {
	unsigned int maxChoice;
	char arrayAnswerKey[NUM_QUESTIONS], arrayStudentAnswers[NUM_STUDENTS][NUM_QUESTIONS];
	int iCounter = 0, questionCounter = 0;
	double answerPenalty, arrayDataStudents[NUM_STUDENTS][3];  //array[..][x] gia x = 0 milame gia ba8mo
	int rightAnswersTemp = 0;								   // gia x = 1 swstes apadhseis
															   // gia x = 2 la8os -//-


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
		scanf("%lf", &answerPenalty);
		if (answerPenalty > 0) {
			printf("Error. Enter non-positive value:\n");
		} 
	} while (answerPenalty > 0);

	for (int studentCounter = 0; studentCounter < NUM_STUDENTS; studentCounter++) {	
		printf("Enter student answers:\n");
		// write k-student's answers (k = studentCounter)
		for(questionCounter = 0, rightAnswersTemp = 0; questionCounter < NUM_QUESTIONS; questionCounter++) {
			scanf(" %c", &arrayStudentAnswers[studentCounter][questionCounter]);
			if (arrayStudentAnswers[studentCounter][questionCounter] - 'A' > maxChoice - 1) {
				do {
					printf("Error. Enter value A-%c\n", maxChoice - 1 + 'A');
					scanf(" %c", &arrayStudentAnswers[studentCounter][questionCounter]);
				} while (arrayStudentAnswers[studentCounter][questionCounter] - 'A' > maxChoice - 1);
			}
			if (arrayStudentAnswers[studentCounter][questionCounter] == arrayAnswerKey[questionCounter]) {
				rightAnswersTemp++;
			}
		}
		// write k-student's grade
		arrayDataStudents[studentCounter][0] = fmax(0, (((double)rightAnswersTemp + (double)(NUM_QUESTIONS - rightAnswersTemp) * (answerPenalty)) / (double)NUM_QUESTIONS) * 10);
	}
	for (int l = 0; l < NUM_STUDENTS; l++) {
		for (int k = 0; k < NUM_QUESTIONS; k++) {
			printf("%c ", arrayStudentAnswers[l][k]);
		}
		printf("%.2lf \n", arrayDataStudents[l][0]);
	}
	return 0;
}
