/*
 * ONOMATEPWNYMO: Tsogkas Panagiotis
 * AEM:03672
 */
#include<stdio.h>
#include"hw1.h"
#include<math.h>

int main (int argc, char *argv[]) {
	unsigned int maxChoice;
	/*static*/ char arrayAnswerKey[NUM_QUESTIONS], arrayStudentAnswers[NUM_STUDENTS][NUM_QUESTIONS];
	char menuInput;
	int iCounterTemp = 0, questionCounter = 0;
	double answerPenalty, arrayDataStudents[NUM_STUDENTS][3];  	//array[..][x] gia x = 0 milame gia ba8mo
	int rightAnswersTemp = 0, gradeCase;						// gia x = 1 swstes apadhseis
	double meanGrade = 0;								   		// gia x = 2 la8os -//-
	double sumUnderSqrt = 0, standardDeviation;
	int arrayGradeGroups[10] = {0}, low, high, sumGradeInRange, lowIndexSelector, highIndexSelector;
	int wrongAnswers = 0;

	do {
		printf("Enter number of choices:\n");
		scanf(" %u", &maxChoice);
	} while (maxChoice >= 27 || maxChoice <= 0);
	printf("Max choice: '%c'\n", maxChoice - 1 + 'A');
	printf("Enter answer key:\n");
	
	for (iCounterTemp = 0; iCounterTemp < NUM_QUESTIONS; iCounterTemp++) {
		scanf(" %c", &arrayAnswerKey[iCounterTemp]);
		if (arrayAnswerKey[iCounterTemp] - 'A' > maxChoice - 1) {
			do {
				printf("Error. Enter value A-%c:\n", maxChoice - 1 + 'A');
				scanf(" %c", &arrayAnswerKey[iCounterTemp]);
			} while (arrayAnswerKey[iCounterTemp] - 'A' > maxChoice - 1);
		}
	}
	printf("Enter wrong answer penalty:\n");
	do {
		scanf("%lf", &answerPenalty);
		if (answerPenalty > 0) {
			printf("Error. Enter non-positive value:\n");
		} 
	} while (answerPenalty > 0);

	printf("Enter student answers:\n");
	for (int studentCounter = 0; studentCounter < NUM_STUDENTS; studentCounter++) {	
		//Write k-student's answers (k = studentCounter)
		for(questionCounter = 0, rightAnswersTemp = 0, wrongAnswers = 0; questionCounter < NUM_QUESTIONS; questionCounter++) {
			scanf(" %c", &arrayStudentAnswers[studentCounter][questionCounter]);
			if ((arrayStudentAnswers[studentCounter][questionCounter] - 'A' > maxChoice - 1) && ((arrayStudentAnswers[studentCounter][questionCounter] != '-'))) {
				do {
					printf("Error. Enter valid answer:\n");
					scanf(" %c", &arrayStudentAnswers[studentCounter][questionCounter]);
				} while ((arrayStudentAnswers[studentCounter][questionCounter] - 'A' > maxChoice - 1) && ((arrayStudentAnswers[studentCounter][questionCounter] != '-')));
			}
			if (arrayStudentAnswers[studentCounter][questionCounter] == arrayAnswerKey[questionCounter]) {
				rightAnswersTemp++;
			}
			else if ((arrayStudentAnswers[studentCounter][questionCounter] != arrayAnswerKey[questionCounter]) && (arrayStudentAnswers[studentCounter][questionCounter] != '-')) {
				wrongAnswers++;
			}
			else;
		}
		//Write number of right-wrong answers
		arrayDataStudents[studentCounter][1] = rightAnswersTemp;
		arrayDataStudents[studentCounter][2] = wrongAnswers;
		//Write k-student's grade
		arrayDataStudents[studentCounter][0] = fmax(0, (((double)rightAnswersTemp + (double)(wrongAnswers) * (answerPenalty)) / (double)NUM_QUESTIONS) * 10.0);
	
	}
	for (int jCounterTemp = 0; jCounterTemp < NUM_STUDENTS; jCounterTemp++) {
		meanGrade += (arrayDataStudents[jCounterTemp][0] / (double)NUM_STUDENTS); 	//Find mean
	}
	for (int jCounterTemp = 0; jCounterTemp < NUM_STUDENTS; jCounterTemp++) {
		sumUnderSqrt += (double)pow(arrayDataStudents[jCounterTemp][0] - meanGrade, 2); //Find standard deviation
	}
	standardDeviation = (double)sqrt(sumUnderSqrt/(double)NUM_STUDENTS);				// -//-
	
	//Categorize based on grade
	for (int jCounterTemp = 0; jCounterTemp < NUM_STUDENTS; jCounterTemp++) {
		gradeCase = (int)floor(arrayDataStudents[jCounterTemp][0]);
		switch (gradeCase) {
			case 0: {
				arrayGradeGroups[0]++;
				break;
			}
			case 1: {
				arrayGradeGroups[1]++;
				break;
			}				
			case 2: {
				arrayGradeGroups[2]++;
				break;
			}
			case 3: {
				arrayGradeGroups[3]++;
				break;
			}
			case 4: {
				arrayGradeGroups[4]++;
				break;
			}
			case 5:	{
				arrayGradeGroups[5]++;
				break;
			}
			case 6: {
				arrayGradeGroups[6]++;
				break;
			}
			case 7:	{
				arrayGradeGroups[7]++;
				break;
			}
			case 8: {
				arrayGradeGroups[8]++;
				break;
			}
			case 9: 
			case 10: {
				arrayGradeGroups[9]++;
				break;
			}
		}
	}
	do {
		//Print menu
		printf("\n[M/m] Print mean\n[S/s] Print standard deviation\n[G/g] Print grade range\n[I/i] Print student info in range\n[H/h] Draw histogram\n[Q/q] Quit\n");
		scanf(" %c", &menuInput);
		switch(menuInput) {
			case 'm': 
			case 'M': {
				//Print mean
				printf("%.2lf", meanGrade);
				printf("\n##\n");
				break;
			}
			case 's':
			case 'S': {
				//Print standard deviation
				printf("%.2lf", standardDeviation);
				printf("\n##\n");
				break;
			}
			case 'g':
			case 'G': {
				//Print how many students in grade range
				sumGradeInRange = 0;
				do {
					printf("Enter grade range:\n");
					scanf("%d-%d", &low, &high);
				} while ((low < 0 || high > 10) || (low >= high));
				for (int jCounterTemp = 0; jCounterTemp < (high - low); jCounterTemp++) {
					sumGradeInRange += arrayGradeGroups[low + jCounterTemp];
				}
				printf("%d", sumGradeInRange);
				printf("\n##\n");
				break;
			}
			case 'i':
			case 'I': {
				//Print students' info in range
				do {
					printf("Enter index range:\n");
					scanf("%d-%d", &lowIndexSelector, &highIndexSelector);
				} while ((lowIndexSelector < 0 || highIndexSelector >= NUM_STUDENTS) || (lowIndexSelector >= highIndexSelector));
				for (int jCounterTemp = 0; (jCounterTemp <= (highIndexSelector - lowIndexSelector)); jCounterTemp++) {
					printf("%03d:%4d,%4d,%4d,%6.2lf\n", lowIndexSelector + jCounterTemp, (int)arrayDataStudents[lowIndexSelector + jCounterTemp][1], 
						  (int)arrayDataStudents[lowIndexSelector + jCounterTemp][2], 
						  (int)(NUM_QUESTIONS - ((arrayDataStudents[lowIndexSelector + jCounterTemp][1] + arrayDataStudents[lowIndexSelector + jCounterTemp][2]))), // find unanswered questions
						   arrayDataStudents[lowIndexSelector + jCounterTemp][0]);
				}
				printf("\n##\n");
				break;
			}
			case 'q':
			case 'Q': {
				//Quit programme
				return (0);
			}
			case 'h':
			case 'H':
				//Draw histogram
				for (int jCounterTemp = 0; jCounterTemp < 10; jCounterTemp++) {
					printf("[%2d,%2d",jCounterTemp, jCounterTemp+1);
					jCounterTemp == 9? printf("]: ") : printf("): ");
					//Group number of students in range in groups of '@' = 100 , '*' = 10 , '-' = 1
					for (int kCounterTemp = 0; kCounterTemp < arrayGradeGroups[jCounterTemp]/100; kCounterTemp++) {
						printf("@");
					}
					for (int kCounterTemp = 0; kCounterTemp < (arrayGradeGroups[jCounterTemp] % 100)/10; kCounterTemp++) {
						printf("*");
					}
					for (int kCounterTemp = 0; kCounterTemp < (arrayGradeGroups[jCounterTemp] % 100)%10; kCounterTemp++) {
						printf("-");
					}
					printf("\n");
				}
				printf("\n##\n");
				break;
			default: {
				printf("Error. Invalid option.\n");
			}
		}
	} while (1); //loop until 'q'
	
	return(-1);
}
