#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{   
    int N = 1000000;
    int num;
    FILE* fptr = fopen("num_file.txt", "w");
    
    if (fptr == NULL)
    {
        printf("ERROR Creating File!");
        exit(1);
    }
    
    srand(time(0));
    for(int i=0;i<N;i++)
    {
        num = rand() % 100;
        fprintf(fptr,"%d ",num);
    }
    fclose(fptr);
    return (0);
}