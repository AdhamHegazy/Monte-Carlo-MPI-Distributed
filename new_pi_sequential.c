
#include <stdio.h>
#include <stdlib.h>

#define ROOTPROCESSID 0
#define TOTALPOINTS 10000     
#define ITERATIONS 50      

//function to generate points and then calculate the value of pi
double generatePoints(int numOfPoints);

int main (int argc, char *argv[])
{
    int processID, numberOfProcesses, processor_len, pointsPerIteration;
    double iteration_pi, pi_estimate, error, temp_pi;
    long double real_pi = 3.14159265358979323846;

    //realistically, we would srand(time(0)) but that would ruin the consistency in tests
    //therefore, we want the seed to be consistent for each run, so we srand(0)
    srand(0);


    pi_estimate = 0.0;
    pointsPerIteration = atoi(argv[1]);
    for (int i=0; i<ITERATIONS; i++)
    {   
        //call generatePoints() every iteration
        iteration_pi = generatePoints(pointsPerIteration); //iteration_pi = value of pi for this iteration only
        pi_estimate = ( (pi_estimate * i) + iteration_pi ) / (i + 1); //cumulative average
        error = pi_estimate - real_pi;

    }
    printf("Value of Pi %.12f with error %.12f\n", pi_estimate, error);

    return 0;
}

double generatePoints(int numOfPoints)
{
    double x, y, pi;

    int pointsWithinCircle = 0;

    for (int i = 0; i < numOfPoints; i++) 
    {
        //x is in interval [-1, 1] (same for y)
        x = (2.0 * (double)rand()/RAND_MAX) - 1.0;
        y = (2.0 * (double)rand()/RAND_MAX) - 1.0;

        //any point that we generate within a circle if its distance to the center <= the radius
        //theoretically, we should take the root of x*x + y*y
        //but since 1's sqrt is also 1, we don't need to
        if (((x*x) + (y*y)) <= 1.0)
        {
            pointsWithinCircle++;
        }
    }

    //the justification behind this equation can be found in the report
    pi = 4.0 * (double)pointsWithinCircle / (double)numOfPoints;
    return(pi);
}