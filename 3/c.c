#include<stdio.h>
#include<omp.h>

#define NUM_THR 8
#define num_steps 400000000

double compute(int j, int size, double step)
{
    int i;  double x,sum;
    for(i=j;i<size;++i)
    {
        x = (i+0.5)*step;
        sum = sum + 4.0/(1.0+x*x);
    }
}

void main()
{
 //CODE (c): #PRAGMA OMP SECTIONS :
      double step,sby2;
      int i;double pi,sum = 0.0;
      step = 1.0/(double)num_steps;
      sby2 = num_steps/2;
      #pragma omp parallel sections
      {
          #pragma omp section
              sum+=compute(0,sby2,step);
          #pragma omp section
              sum+=compute(sby2,num_steps,step);
      }
      pi = step*sum;
      printf("%f", pi);
}