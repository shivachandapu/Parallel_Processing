#include<stdio.h>
#include<math.h>
#include<omp.h>

int alpha()
{
    return 10;
}

int beta()
{
    return 10;
}

int delta()
{
    return 10;
}

double gamma1(double a,double b)
{
    return a*b ;
}

double epsilon(double x,double y)
{
    return x+y;
}
void main()
{
    double v,w,y,x;
    #pragma omp parallel sections
     {
     #pragma omp section
         v=alpha();
     #pragma omp section
         w=beta();
     #pragma omp section
         y=delta();
     }
       x = gamma1(v,w);       
    printf("%6.2f\n",epsilon(x,y));
}