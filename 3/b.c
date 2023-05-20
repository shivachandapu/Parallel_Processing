#include<stdio.h>
#include<omp.h>

#define num_threads 8
#define num_steps 400000000
void main()
{
//CODE (b): #PRAGMA OMP FOR :
   double step;
      int i;double pi,sum = 0.0;
      step = 1.0/(double)num_steps;
      #pragma omp parallel 
      {
          double x;
          #pragma omp for reduction(+:sum) 
          for(i=0;i<num_steps;++i)
          {
              x = (i+0.5)*step;
              sum = sum + 4.0/(1.0+x*x);
          }
      }
      pi = step*sum;
      printf("%f", pi);
}