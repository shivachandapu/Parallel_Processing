#include <stdio.h>
#include<omp.h>
#include<math.h>
#include<stdlib.h>
#define number 100000000
#define NUM_THR 128

void main()
{

    long sqrtOfNumber = (long)sqrt(number);
    long *sieves = malloc(sqrtOfNumber*sizeof(long)),*numbersArray = malloc(number*sizeof(long));
    int i=0;
    double start,end;

    //calculating all sieves for the given number
    for(i=2;i<=sqrtOfNumber;++i)
    {
        sieves[i-2] = i;
    }
    //Array containing all the numbers
    for(i=0;i<number;++i)
    {
        numbersArray[i] = i;
    }
    //Timer start
    start = omp_get_wtime();
    omp_set_num_threads(NUM_THR);
    #pragma omp parallel shared(sieves,numbersArray)
    {
        int startIndex,endIndex,sizeOfBlock,first;
        long sieve;
        int id=omp_get_thread_num();
        // int NUM_THR = omp_get_num_threads();
        int i,j;
        startIndex = (id*number)/NUM_THR ;
        endIndex = (((id+1)*number)/NUM_THR)-1;
        
        #pragma omp for 
        for(j=0;j<sqrtOfNumber-2;++j)
        {
            sieve = sieves[j];
            if(sieve*sieve>startIndex)
                first = sieve*sieve;
            else{
                if(!(startIndex%sieve))
                    first = startIndex;
                else
                    first = startIndex+(sieve - (startIndex%sieve));
            }
            for ( i = first; i <=endIndex; i+=sieve )
                numbersArray[i]=0;
        }        
    }
    //Timer ends
    
    end = omp_get_wtime()-start;
    printf("computed in %.4g seconds ",end);
} 
