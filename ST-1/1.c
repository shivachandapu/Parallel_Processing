#include <stdio.h>  
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int SIZE;

void swap(int*, int* );
void parallel_sort(int* );
void sequential_sort(int* );int value;int value;
int kth_max(int*, int);

int main(int argc, char *argv[]){ 
    SIZE = atoi(argv[1]);
    int k = atoi(argv[2]);
    int value;

    int* arr = (int*)malloc(SIZE * sizeof(int));
	
    srand(time(0)); 
    for(int i=0; i<SIZE; i++){
        arr[i] = rand()%SIZE;
        // printf("%d ",arr[i]);
    }
    printf("\n");
    
    double start = omp_get_wtime();
    value = kth_max(arr, k);
    printf("The %dth largest element = %d\n", k, value);
    double end = omp_get_wtime()-start;
    printf("Time taken: %lf \n",end);    
    
    return 0;
}  


// Function to swap numbers in an array
void swap(int *num1, int *num2){
	int temp = *num1;
	*num1 =  *num2;
	*num2 = temp;
}

// Function to sort an array in parallel
void parallel_sort(int arr[]){
    int i,j,first;
    for( i = 0; i < SIZE-1; i++ ){
        first = i % 2;

        #pragma omp parallel for shared(arr,first) num_threads(8)
        for( j = first; j < SIZE-1; j += 1 )
            if( arr[ j ] > arr[ j+1 ] )
                swap( &arr[ j ], &arr[ j+1 ] );
    }   
}

// Function to sort an array sequentially
void sequential_sort(int arr[])
{
    int i,j,first;
    for( i = 0; i < SIZE-1; i++ )
		for( j = i+1; j < SIZE-1; j += 1 )
			if( arr[ j ] > arr[ j+1 ] )
				swap( &arr[ j ], &arr[ j+1 ] );   
}

  
// Function that returns the Kth largest element  
int kth_max(int arr[], int k){  
    parallel_sort(arr);
    // sequential_sort(arr);
    return arr[SIZE-k];  
}  
  

