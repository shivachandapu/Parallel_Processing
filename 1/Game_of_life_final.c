/*
"The Game of Life, also known simply as Life, is a cellular automaton devised by the British
mathematician John Horton Conway in 1970."
The board is made up of an m x n grid of cells, where each cell has an initial state: live (represented
by a 1) or dead (represented by a 0). Each cell interacts with its eight neighbors (horizontal, vertical,
diagonal) using the following four rules (taken from the above Wikipedia article):
1. Any live cell with fewer than two live neighbors dies as if caused by under-population.
2. Any live cell with two or three live neighbors lives on to the next generation.
3. Any live cell with more than three live neighbors dies, as if by over-population.
4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
The next state is created by applying the above rules simultaneously to every cell in the current state,
where births and deaths occur simultaneously. Given the current state of the m x n grid board,
return the next state.

By: Chandapu Shiva Krishna
1st MTech-CS
Regd no: 22555
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int** create_mat(int,int);

int main(int argc, char* argv[]){
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int **input_mat, **output_mat;
    // printf("\nEnter the n of the matrix: ");
    // scanf("%d%d", &m, &n);
    
    input_mat = create_mat(m+2,n+2);  //creating a padded matrix
    output_mat = create_mat(m+2,n+2);
    
    // checking the neighbours and creating the next generation.
    int i, j, p, q, live_neibor;
    
    // clock_t start = clock();
    for(i = 1; i<m+1; i++){
        for(j = 1; j<n+1; j++){                
            live_neibor = 0;
            for(p=i-1; p<i+2; p++)
                for (q=j-1; q<j+2; q++)
                    if(!(p==i && q==j) && input_mat[p][q]) 
                        live_neibor++;
           
            if((input_mat[i][j] == 1 && (live_neibor == 2 || live_neibor == 3)) || (input_mat[i][j] == 0 && live_neibor == 3)) 
                output_mat[i][j] = 1;
            else 
                output_mat[i][j] = 0;
        }    
    }
    // clock_t stop = clock();
    // printf("Time taken: %f\n",(double)(stop - start)/CLOCKS_PER_SEC);
    
    // printf("\n");
    for(int i=1; i<m+1; i++){
        for(int j=1; j<n+1; j++)
            printf("%d ",input_mat[i][j]);
        printf("\t\t");
        for(int j=1; j<n+1; j++)
            printf("%d ",output_mat[i][j]);
        printf("\n");
    }

    free(input_mat);
    free(output_mat);
    return 0;
}

int** create_mat(int p,int q){
    int** mat;
    srand(time(0));
    mat = (int**)malloc(q*sizeof(int*));
    
    for(int j=0;j<q;j++)
       mat[j] = (int*)malloc(p*sizeof(int));
    
    for(int i=0; i<p; i++)
        for(int j=0; j<q; j++){
            if(i==0 ||i==p-1 || j==0 || j==q-1)
                mat[i][j]=0;
            else
                mat[i][j] = rand() % 2;
        }
    return mat;
}
