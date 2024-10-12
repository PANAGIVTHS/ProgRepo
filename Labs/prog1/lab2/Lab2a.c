
//Find the distance that that the firework will cover before exploding 
//for any given values of initialVelocity, fuseTimer

#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) {

    double initialVelocity, fuseTimer, boomDistance;
    const double gravAccel = -9.8;
    char timeMetric;

    printf("Initial velocity (km/h):\n");
    scanf("%lf", &initialVelocity);
    initialVelocity = initialVelocity * 5/18;                                                           //converts value from km/h to m/s

    printf("Fuse timer:\n");
    scanf(" %lf%c", &fuseTimer, &timeMetric);
    
    fuseTimer = ((timeMetric == 'm')? (fuseTimer*60) : (fuseTimer));                                    // converts time value to sec
    boomDistance = (initialVelocity * fuseTimer) + (gravAccel * 0.5 * fuseTimer * fuseTimer);           // Distance formula
    printf("Boom at %.2lfm!\n", boomDistance);

    return(0);
}