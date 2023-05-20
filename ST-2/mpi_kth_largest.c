// Program to find the kth largest element in the array

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

// Merge Function
void merge(int *a, int *b, int l, int m, int r) 
{
	int h, i, j, k;
	h = l;
	i = l;
	j = m + 1;
	while((h <= m) && (j <= r)) {
		if(a[h] <= a[j]) {
			b[i] = a[h];
			h++;
			}
		else {
			b[i] = a[j];
			j++;
			}		
		i++;		
	}
	if(m < h) {
		for(k = j; k <= r; k++) {
			b[i] = a[k];
			i++;
			}	
	}
	else {
		for(k = h; k <= m; k++) {
			b[i] = a[k];
			i++;
			}	
	}
	for(k = l; k <= r; k++)
		a[k] = b[k];
}

// Recursive Merge Function
void mergeSort(int *a, int *b, int l, int r) 
{	
	int m;	
	if(l < r) {		
		m = (l + r)/2;		
		mergeSort(a, b, l, m);
		mergeSort(a, b, (m + 1), r);
		merge(a, b, l, m, r);		
	}		
}

// Main function
int main(int argc, char** argv) 
{

	int n = atoi(argv[1]); 		//size of the array
	int k = atoi(argv[2]); 		//kth largest element

	int *arr = malloc(n * sizeof(int));
	int reminder,c;
	FILE* f=fopen(argv[3],"r"); // input text file
	if(f==NULL)
	{
		printf("file not found\n");
		exit(0);
	}
	//reading the array from the file
	for(c = 0; c < n; c++)
		fscanf(f,"%d",&arr[c]);

	// Initialize MPI
	int rank;
	int world_size;
	double start,end;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	if(rank==0) {
		printf("\n");
		if(k>n){
			printf("k cannot be greater than n\n");
			MPI_Finalize();
			exit(0);
		}
		start=MPI_Wtime();
		reminder=n%world_size;
	}
		
	// Divide the array in equal-sized chunks
	int size = n/world_size;
	
	// Send each subarray to each process
	int *sub_array = malloc(size * sizeof(int));
	MPI_Scatter(arr, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);
	
	// Perform the mergesort on each process
	int *temp = malloc(size * sizeof(int));
	mergeSort(sub_array, temp, 0, (size - 1));
	
	// Gather the sorted sub-arrays into one
	int *sorted_array = NULL;
	if(rank == 0){
		sorted_array = malloc(n * sizeof(int));
	}
	MPI_Gather(sub_array, size, MPI_INT, sorted_array, size, MPI_INT, 0, MPI_COMM_WORLD);
	
	//  Make the final mergeSort call
	if(rank == 0) 
	{	
		int *other_array = malloc(n * sizeof(int));
		for(c=1;c<=reminder;c++)
			sorted_array[n-c] = arr[n-c];
		mergeSort(sorted_array, other_array, 0, (n - 1));
		printf("\n\n");
		printf("The k'th largest number an array of size %d from the input file is: %d\n",n,sorted_array[n-k]);
		free(sorted_array);
		free(other_array);
	}
	free(arr);
	free(sub_array);
	free(temp);
	
	if(rank==0)	{
		end=MPI_Wtime();
		printf("time taken: %f\n",(end-start));
	}

	// Finalize MPI
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	
}