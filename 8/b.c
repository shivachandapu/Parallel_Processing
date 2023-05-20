// Pragma omp task

#include<stdio.h>
#include<omp.h>

#define NUM_THR 4
#define num_steps 400000000

int fib(int n)
{
    int x,y;
    if(n<2) 
        return n;
    #pragma omp task shared(x)
        x = fib(n-1);
    #pragma omp task shared(y)
        y = fib(n-2);
    #pragma omp taskwait
        return x+y;
}

void main()
{
#pragma omp task
   { int n = 25;
    n = fib(n);
    printf("%d\n",n);

   }
}
