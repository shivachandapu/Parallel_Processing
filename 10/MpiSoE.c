/*To run the program
 *compile: mpicc MpiSoE.c -lm -o smpi
 *run: mpirun -n "number of processes" ./smpi "size till where we want the prime number"
 *eg:mpirun -n 8 ./a.out 50
 */

#include<math.h>
#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#define BLOCK_LOW(id,p,n) ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n) (BLOCK_LOW((id)+1,p,n)-BLOCK_LOW(id,p,n))
#define BLOCK_OWNER(index,p,n) ((p)*((index+1)-1)/(n))
#define MIN(a,b) ((a)<(b) ? (a):(b))

int main(int argc,char *argv[])
{
	int count;
	double elapsed_time;
	int first;
	int global_count;
	int high_value;
	int i;
	int id;
	int index;
	int low_value;
	char *marked;
	int n;
	int p;
	int proc0_size;
	int prime;
	int size;
	MPI_Init(&argc,&argv);
	MPI_Barrier(MPI_COMM_WORLD);
	elapsed_time=MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	if(argc!=2){
		if(!id)
			printf("Command line: %s<m>\n",argv[0]);
		MPI_Finalize();
		exit(1);
	}
	n=atoi(argv[1]);
	low_value=2+BLOCK_LOW(id,p,n-1);
	high_value=2+BLOCK_HIGH(id,p,n-1);
	size=BLOCK_SIZE(id,p,n-1);
	proc0_size=(n-1)/p;
	
	if((2+proc0_size)<(int) sqrt((double) n)){
		if(!id)
			printf("Too many processors\n");
		MPI_Finalize();
		exit(1);
	}
	
	marked=(char *)malloc(size);
	
	if(marked==NULL)
	{
		printf("Cannot allocate enough memory\n");
		MPI_Finalize();
		exit(1);
	}
	
	for(i=0;i<size;i++)
		marked[i]=0;
	if(!id)
		index=0;
	prime=2;
	
	do{
		if(prime*prime>low_value)
			first=prime*prime-low_value;
		else{
			if(!(low_value%prime))
				first=0;
			else
				first=prime-(low_value%prime);
		}
		for(i=first;i<size;i+=prime)
			marked[i]=1;
		if(!id){
			while(marked[++index]);
			prime=index+2;
		}
		MPI_Bcast(&prime,1,MPI_INT,0,MPI_COMM_WORLD);
	}while(prime*prime<=n);
	
	count=0;
	for(i=0;i<size;i++)
		if(!marked[i]) count++;
	MPI_Reduce(&count,&global_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	elapsed_time+=MPI_Wtime();
	
	if(!id){
		printf("%d primes are less than or equal to %d\n",global_count,n);
		printf("Total elapsed time:%10.6f\n",elapsed_time);
	}
	MPI_Finalize();
	return 0;
}
