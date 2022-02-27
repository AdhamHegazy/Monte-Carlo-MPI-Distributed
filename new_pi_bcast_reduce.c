#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROOTPROCESSID 0   
#define ITERATIONS 50      

//function to generate points and then calculate the value of pi
double generatePoints(int numOfPoints);

int main (int argc, char *argv[])
{
    int processID, numberOfProcesses, processor_len, n, pointsPerIteration;
    double process_pi, pi_sum, pi_estimate, error, final_process_pi;
    long double real_pi = 3.14159265358979323846;
    char processor_name[1000];

    MPI_Init(NULL, NULL);
    
    MPI_Comm_size(MPI_COMM_WORLD,&numberOfProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD,&processID);

    //we dont really use this in the final code but we did initially to test that processes ran
    //on different processors
    // MPI_Get_processor_name(processor_name, &processor_len);

    //realistically, we would srand(time(0)) but that would ruin the consistency in tests
    //therefore, we want the seed to be consistent for each run but different for each process
    srand(processID);


    if (processID == 0)
    {
        pointsPerIteration = atoi(argv[1]);
        n = pointsPerIteration/numberOfProcesses;
    }

    MPI_Bcast(&n, 1, MPI_INT, ROOTPROCESSID, MPI_COMM_WORLD);

    pi_estimate = 0.0;
    process_pi = 0.0;
    final_process_pi = 0.0;
    for (int i=0; i<ITERATIONS; i++)
    {   
        //each process calls generatePoints() to calculate an estimate of the value pi
        process_pi += generatePoints(n);

        // printf("Process #%d on processor '%s'... calculated pi = %f\n", processID, processor_name, process_pi);
    }

    final_process_pi = process_pi/(double)ITERATIONS;
    MPI_Reduce(&final_process_pi, &pi_sum, 1, MPI_DOUBLE,
                MPI_SUM, ROOTPROCESSID, MPI_COMM_WORLD);

    //root process computes the mean
    if (processID == ROOTPROCESSID)
    {
        pi_estimate = pi_sum / numberOfProcesses; //estimate = average of each process's estimate
        error = pi_estimate - real_pi;
    }
    
    if (processID == ROOTPROCESSID)
        printf("Value of Pi %.12f with error %.12f\n", pi_estimate, error);

    MPI_Finalize();
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
        //but since 1 sqaured is also 1, we don't need to
        if (((x*x) + (y*y)) <= 1.0)
        {
            pointsWithinCircle++;
        }
    }

    //the justification behind this equation can be found in the report
    pi = 4.0 * (double)pointsWithinCircle / (double)numOfPoints;
    return(pi);
}