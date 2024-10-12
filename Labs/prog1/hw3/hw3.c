/* 
	This programme schedules the weekly schedule for a given 
	number of classrooms. Takes parameters like room capacity,
	professor name, lesson info ect...
*/

#include"hw3.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum {false, true} boolean;

// Weekdays
typedef enum {Mon, Tue, Wed, Thu, Fri, Sat, Sun} days;

typedef struct lesson {
	int identifier, numOfStudents;
	char professorName[MAX_NAME_LEN];
} coursesT;

typedef struct classrooms {
	int classSize;
	coursesT *schedule[WORKING_HOURS][WORKING_DAYS];
} classesT;

// This function handles all programme parameters (given by terminal)

int cmdArgumentHandler (char *argv[], int argc) {
	int prevValueTemp;
	
	// Check if no arguments
	if (argc == 1) {
		printf("Incorrect command-line arguments!\n");
		return (-1);
	} 	

	// Check if there is an argument "overflow"
	if (argc - 1 > MAX_ROOMS) {
		printf("Incorrect command-line arguments!\n");
		return (-1);
	}

	// Make sure numbers are in ascending order (and positive intigers)
	for (int i = 1; i < argc; i++) {
		if (atoi(argv[i]) <= 0 || ((atoi(argv[i]) < prevValueTemp) && (i != 1))) {
			printf("Incorrect command-line arguments!\n");
			return(-1);
		}
		prevValueTemp = atoi(argv[i]);
	}
	return(0);
}

// Initialise arrays of type coursesT
void initialiseCourses (coursesT *array) {
	for (int i = 0; i < MAX_COURSES; i++) {
		array[i].identifier = 0;
		array[i].numOfStudents = 0;
	}
}

//Initialise arrays of type classesT
void initialiseClasses (classesT *array) {
	for (int i = 0; i < MAX_ROOMS; i++) {
		// initialise class sizes
		array[i].classSize = 0; 
		// initialise schedule 
		for (int j = 0; j < WORKING_DAYS; j++) {
			for (int k = 0; k < WORKING_HOURS; k++) {
				array[i].schedule[k][j] = NULL; 
			}
		}
	}
}

// This function searches for a given course in the list of courses
// Returns address where it was found, first empty/'strorable' address,
// NULL in case none of the above is true  

coursesT *searchForCourse (coursesT *coursesArray, int courseId) {
	coursesT *firstNULLptr = NULL;

	for (int i = 0; i < MAX_COURSES; i++) {
		// Course found
		if (coursesArray[i].identifier == courseId) {
			// Return address
			return(&(coursesArray[i]));
		}
		if ((firstNULLptr == NULL) && (coursesArray[i].identifier == 0)) {
			// Find first NULL address
			firstNULLptr = &coursesArray[i];
		}
	}

	// return NULL if no free space, or first NULL address
	return(firstNULLptr);
}

// this function adds a course to the list of courses which are already stored
// Returns (0) if course exists, (-1) if it doesnt exist + no space to store it,
// (1) if addition was successful 

int addCourse (coursesT *coursesArray, int courseId, int studentNum, char* profName) {
	coursesT *coursePtr = NULL;

	coursePtr = searchForCourse(coursesArray, courseId);
	// Check if NULL then check if lesson was found in array
	if ((coursePtr != NULL) && ((*coursePtr).identifier == courseId)) {
		// Found
		return(0);
	}
	// not Found + no free space
	else if (coursePtr == NULL) {
		return(-1);
	}
	// not Found + free space
	else {
		// Copy data to struct
		(*coursePtr).identifier = courseId;
		(*coursePtr).numOfStudents = studentNum;
		strcpy((*coursePtr).professorName, profName);
	}

	return(1);
}

// This function prints all stored courses (Professor name, number of students, identifier)
// no return type.

void printCourses (coursesT *coursesArray) {
	coursesT *coursePtr = coursesArray;
	boolean foundCourse = false;

	// coursePtr - coursesArray: how many loops 
	for (; coursePtr - coursesArray < MAX_COURSES; coursePtr++) {
		if (coursePtr->identifier != 0) {
			foundCourse = true;
			printf("[%d] %s %d\n", coursePtr->identifier, coursePtr->professorName, coursePtr->numOfStudents);
		}
	}
	// If no courses are found then say so.
	if (foundCourse == false) {
		printf("No courses.\n");
	}
}

// This function Checks if the professor is available for any given time. Returns true/false

boolean areHoursValid (classesT *classInfo, coursesT *coursePtr, int duration, days day, int hourIndex) {
	// Something went wrong return false
	if (hourIndex < 0) return(false);
	for (int i = 0; i < MAX_ROOMS; i++) {	/*classroom counter*/
		// make sure all hours are valid
		for (int hourCounter = hourIndex; hourCounter-hourIndex < duration; hourCounter++) { 
			// Check if the professor has a lecture in another classroom 
			if (((classInfo[i].schedule[hourCounter][day]) != NULL) && 
			    strcmp((classInfo[i].schedule[hourCounter][day])->professorName,coursePtr->professorName) == 0) {
				return(false);
			}
		}
	}
	return(true);
}

// This function finds for each day the earliest hour a lesson can be scheduled. Then finds
// the earliest hour for all working days. Returns -1 if nothing is found else it returns 
// the classroom where the lesson was scheduled. Returns the classroom where the lecture
// was scheduled in

int lectureCalculator (classesT *classInfo, coursesT *coursePtr, int duration) {
	int consecutiveHours = 0;
	boolean timeFound = false, isValid = false, foundOnce = false;
	int earliestHour = 404;
	int earliestCurrentHour = 404;
	// Sundays we are closed (Thank god)
	days dayFoundIn = Sun;
	// i represents the classrooms
	int i;

	for (i = 0; i < MAX_ROOMS; i++) {
		// If there is not enough space check next classroom
		if (classInfo[i].classSize < coursePtr->numOfStudents) continue;
		// Found classroom
		for (days dayCounter = Mon; dayCounter < WORKING_DAYS; dayCounter++) {
			consecutiveHours = 0;
			timeFound = false;
			for (int currentHour = 0; currentHour < WORKING_HOURS; currentHour++) {
				// if this is == NULL that means the room is free at this time 
				if (classInfo[i].schedule[currentHour][dayCounter] == NULL) {
					consecutiveHours++;
				}
				else {
					// if the room isnt free reset the counter
					consecutiveHours = 0;
				}
				// currentHour-duration+1 is the start of lecture
				// possible false-positive. Check if true-positive
				isValid = areHoursValid(classInfo, coursePtr, duration, dayCounter, currentHour-duration+1);
				if ((consecutiveHours == duration) && (isValid)) {
					timeFound = true;
					foundOnce = true;

					earliestCurrentHour = currentHour-duration+1;
					// Find earliest available hour for the whole week
					if (earliestCurrentHour < earliestHour) {
						earliestHour = earliestCurrentHour;
						dayFoundIn = dayCounter;
					}	
					break;
				}
				else if ((consecutiveHours == duration) && (isValid == false)) {
					// decrease by one so next loop checks next index 
					consecutiveHours--;
					timeFound = false;
				}
				// optimal/earliest time for this day has been found. Move on to next day
				if (timeFound) break;
			}
		}
		if (foundOnce) break;
	}
	// i-1 because of for's last loop
	if (i == 3) i = 2; 
	// if cannot be scedualed return -1
	if (earliestHour == 404) {
		return(-1);
	}
	// Set classroom info (Fill schedule)
	for (int j = 0; j < duration; j++) {
		(classInfo[i].schedule[earliestHour+j][dayFoundIn]) = coursePtr;
	}
	
	// return classroom index
	return(i);
}

// This function prints the schedule for each one of the classrooms
// no return type.

void printClassSchedule (classesT *classPtr) {
	printf("\n[%d]\n", classPtr->classSize);
	printf("      ");
	// print however many days needed (WORKING_DAYS)
	for (days dayCounter = 0; dayCounter < WORKING_DAYS; dayCounter++) {
		switch (dayCounter) {
			case Mon: {
				printf(" MON");
				break;
			} 
			case Tue: {
				printf(" TUE");
				break;
			}
			case Wed: {
				printf(" WED");
				break;
			}
			case Thu: {
				printf(" THU");
				break;
			}
			case Fri: {
				printf(" FRI");
				break;
			}
			default:{
				break;
			}
		}
	}
	printf("\n");
	for (int hourCounter = 0; hourCounter < WORKING_HOURS; hourCounter++) {
		// print actual time earliest is 10 so add 10 to the counter
		printf("%d:00  ", hourCounter + 10);
		for (days dayCounter = 0; dayCounter < WORKING_DAYS; dayCounter++) {
			// Print - if schedule is empty (not filled yet or deleted course)
			if (classPtr->schedule[hourCounter][dayCounter] == NULL || classPtr->schedule[hourCounter][dayCounter]->identifier == 0) {
				(printf(" -  "));
			}
			else {
				printf("%d ", (classPtr->schedule[hourCounter][dayCounter])->identifier);
			}
		}
		printf("\n");
	}
}

// This function prints all classes' schedule
// no return type.

void printWeeklySchedule (classesT *classInfo) {
	for (int i = 0; i < MAX_ROOMS; i++) {
		printClassSchedule(&classInfo[i]);
	}
}

// This function deletes all appearances of a particular course from the weekly schedule
// no return type.

void deleteLectures (classesT *classInfo, int identifierInput) {
	/* Check ALL indexes of the schedule*/

	// Check in every classroom's schedule
	for (int i = 0; i < MAX_ROOMS; i++) {
		// Check every day
		for (days dayCounter = 0; dayCounter < WORKING_DAYS; dayCounter++) {
			// Check every hour
			for (int hourCounter = 0; hourCounter < WORKING_HOURS; hourCounter++) {
				if (((classInfo[i].schedule[hourCounter][dayCounter]) != NULL) && 
					((classInfo[i].schedule[hourCounter][dayCounter])->identifier == identifierInput)) {
					// When found delete (overwrite with NULL address)
					(classInfo[i].schedule[hourCounter][dayCounter]) = NULL;
				}
			}
		}
	}
}

// This function deletes a course from coursesArray
// and restores all variables (of that course) to their original values
// returns (0) if the course wasnt found and (1) if it was 

int deleteCourse (coursesT *coursesArray, int identifierInput, classesT *classInfo) {
	char blankStr[MAX_NAME_LEN] = {0};
	boolean foundCourse = false;
	int i;

	// Search for course
	for (i = 0; i < MAX_COURSES; i++) {
		if (coursesArray[i].identifier == identifierInput) {
			// Found
			foundCourse = true;
			break;
		}
	}
	// delete from schedule
	deleteLectures(classInfo, identifierInput);
	// reset values
	if (foundCourse) {
		coursesArray[i].numOfStudents = 0;
		coursesArray[i].identifier = 0;
		strcpy(coursesArray[i].professorName, blankStr);	
	}
	return(foundCourse);
} 

int main (int argc, char *argv[]) {
	coursesT coursesArray[MAX_COURSES];
	classesT classInfo[MAX_ROOMS];
	coursesT *coursePtr;
	int courseId, studentNum, addCourseStatus, duration;
	int lectureStatus;
	char profName[MAX_NAME_LEN];
	char formatStr[40];
	char menuInput;

	sprintf(formatStr, " %%%ds %%d %%d", MAX_NAME_LEN-1);

	// End programme if arguments are wrong
	if (cmdArgumentHandler(argv, argc)) {
		return(-1);
	}

	// Initialise arrays
	initialiseClasses(classInfo);
	initialiseCourses(coursesArray);

	// Fill classInfo array with class sizes
	for (int i = 1; i < argc; i++) {
		classInfo[i-1].classSize = atoi(argv[i]);
	}

	do {
		// Prints user interface
		print_menu();
		// Wait for input
		scanf(" %c", &menuInput);

		switch (menuInput) {
			case 'q': {
				// Quit
				return(0);
			}
			case 'a': {
				// Attempt to add a course

				printf("Professor course students:\n"); 
				scanf(formatStr, profName, &courseId, &studentNum);
				// Add course if it doesnt exist 
				addCourseStatus = addCourse(coursesArray, courseId, studentNum, profName);
				if (addCourseStatus == 0) {
					printf("%d exists.\n", courseId);
				} 
				else if (addCourseStatus == 1) {
					printf("%d added.\n", courseId);
				} 
				else {
					// Cannot be added
					printf("No space.\n");
				}
				break;
			}
			case 'c': {
				// Print all stored courses
				printCourses(coursesArray);
				break;
			}
			case 's': {
				// Attempt to scedule a lesson
				scanf("%d", &courseId); // Wait for the courses identifier to be given
				printf("Course duration:\n");
				scanf("%d", &duration);

				// duration cant be < 1
				if (duration < 1) {
					printf("Invalid duration.\n");
				}
				else {
					coursePtr = searchForCourse(coursesArray, courseId);
					if ((coursePtr == NULL) ||((coursePtr != NULL) && ((*coursePtr).identifier != courseId))) {
						printf("%d not found.\n", courseId);
					}
					else {
						lectureStatus = lectureCalculator(classInfo, coursePtr, duration);
						if (lectureStatus == -1) {
							// couldnt schedule the lecture
							printf("%d not scheduled.\n", courseId);
						}
						else {
							// Successfully scheduled the lecture
							printf("%d scheduled in %d.\n", courseId, lectureStatus);
						}
					}
				}
				break;
			}
			case 'p': {
				// Print the weekly schedule (all classes' schedule)
				printWeeklySchedule(classInfo);
				break;
			}
			case 'r': {
				// Remove a course 
				printf("Course:\n");
				scanf("%d", &courseId);
				if (deleteCourse(coursesArray, courseId, classInfo)) {
					printf("%d deleted.\n", courseId);
				}
				else {
					printf("%d not deleted.\n", courseId);
				}
				break;
			}
			default: {
				// Wrong input
				printf("Invalid choice.\n");
				continue;
			}
		}
	} while(1);

	return(0);
}