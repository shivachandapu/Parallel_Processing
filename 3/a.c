#include<stdio.h>
#include<omp.h>

#define num_threads 8
#define num_steps 400000000

void main()
{
    //#PRAGMA OMP PARALLEL:
     double step;
     int i,nthreads=0;    double pi,sum[num_threads][8];
     step = 1.0/(double)num_steps;
     omp_set_num_threads(num_threads);
     #pragma omp parallel 
     {
         int i,id,nthrds;
         double x;
         id = omp_get_thread_num();
         nthrds = omp_get_num_threads();
         if(id==0) nthreads=nthrds;
         for(i=id,sum[id][0]=0.0;i<num_steps;i+=nthrds)
         {
             x = (i+0.5)*step;
             sum[id][0] = sum[id][0] + 4.0/(1.0+x*x);
         }
     }
     for(i=0;i<nthreads;++i)
         pi += step*sum[i][0];
     printf("num threads : %d\n",nthreads);
     printf("%f", pi);
}