
// Find the definite Intergral of a given function (eval)

#include<stdio.h>
#include<math.h>

// Function Headers
/* Function to integrate. */
double eval(double x);
double calTrapezoidArea (double smallBase, double bigBase, double height);
double calIntergreal (double firstPoint, double secondPoint, int numOfIntervals);
double approximateIntregral (double firstPoint, double secondPoint, double allowedError, int maxNumIntervals);
void percentError (int intervalCounter, int maxNumIntervals);

int main (int argc, char *argv[]) {
    double firstPoint, secondPoint;
    int maxNumIntervals, intervalCounter;

    printf("Interval:\n");
    scanf("%lf-%lf", &firstPoint, &secondPoint);
    printf("Max slices:\n");
    scanf(" %d", &maxNumIntervals);
    // Find number of intervals/slices used
    intervalCounter = approximateIntregral(firstPoint, secondPoint, 0.01, maxNumIntervals);
    printf("Slices: %d\n", intervalCounter);
    percentError(intervalCounter, maxNumIntervals);

	return 0;
}

double calTrapezoidArea (double smallBase, double bigBase, double height) {
    double resultArea;
    // Find the area of a given trapezoid
    resultArea = ((smallBase + bigBase)*height)/2.0;
    return(resultArea);
}

double calIntergreal (double firstPoint, double secondPoint, int numOfIntervals) {
    double intervalLength, smallBase, bigBase;
    double area = 0;

    // Find the length of each interval (All intervals have the same length)
    intervalLength = (secondPoint - firstPoint) / (double)numOfIntervals;
    for (int i = 0; i < numOfIntervals; i++) {
        smallBase = eval(firstPoint + i*intervalLength); // firstPoint += intervLeng also works
        bigBase = eval(firstPoint + (i+1)*intervalLength);
        // Find the integral 
        area += calTrapezoidArea (smallBase, bigBase, intervalLength);
    }
    return(area);
}

double approximateIntregral (double firstPoint, double secondPoint, double allowedError, int maxNumIntervals) {
    double resValue = 0, resValueOld = 0;
    double deviation = 0;
    // int counter here so as to return its value
    int intervalCounter = 1;

    for (; intervalCounter <= maxNumIntervals; intervalCounter++) {
        resValueOld = resValue;
        resValue = calIntergreal(firstPoint, secondPoint, intervalCounter);
        deviation = fabs(resValueOld - resValue);
        printf("%2d: %6.2lf %6.2lf\n", intervalCounter, resValue, deviation);
        // Check if deviation is within the accepted limit for Error.
        if (deviation <= allowedError) break;  
    }
    if (intervalCounter > maxNumIntervals) {
        // Edge case of max number of intervals reached
        intervalCounter--;
    }
    return(intervalCounter);
}

void percentError (int intervalCounter, int maxNumIntervals) {
    double percent;

    // Find % of intervals of maxNumber of intervals
    percent = ((double)intervalCounter / (double)maxNumIntervals)*100.0;
    printf("Percent: %.2lf%%\n", percent);
    if(percent > 75) {
        printf(">75%%\n");
    } else if (percent < 25) {
        printf("<25%%\n");
    } else {
        printf("25-75%%\n");
    }
}