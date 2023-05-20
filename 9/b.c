// * pragma omp atomic

//   The omp atomic directive allows access of a specific memory location atomically. It ensures that race conditions are avoided through direct control 
//   of concurrent threads that might read or write to or from the particular memory location.
//   The atomic construct ensures that a specific storage location is updated atomically, 
//   rather than exposing it to the possibility of multiple, simultaneous writing threads.

#include <stdio.h>
#include <omp.h>

#define NUM_THR 4
#define num_steps 400000000

void main()
{
    double step;
    int i,nthreads;double pi;
    step = 1.0/(double)num_steps;
    omp_set_num_threads(NUM_THR);
    double start = omp_get_wtime();
#pragma omp parallel shared(nthreads)
    {
       int i,id; 
       double x,sum;
       id=omp_get_thread_num();
       #pragma omp master
       {
            nthreads=omp_get_num_threads();
       }
       
        for(i=id,sum=0.0;i<num_steps;i=i+nthreads)
        {
            // printf("%d", omp_get_num_threads());
            x = (i+0.5)*step;
            sum+= 4.0/(1.0+x*x);
             
        }
        sum =sum*step;
        #pragma omp atomic
        pi+=sum;
        double delta = omp_get_wtime()-start;
    printf("PI = %.16g computed in %.4g seconds with %d threads.\n", pi, delta,NUM_THR);
    }
}