#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int size;

int** create_mat();
int** matrix_mult(int** A, int** B);
void print_matrix(int **mat);
void free_matrix(int **mat);
void print_mat(int** mat);

int main(int argc, char *argv[]){
    size = atoi(argv[1]);
    int **A, **B, **C;

    A = create_mat();
    B = create_mat();
    
    clock_t start = clock();
    C = matrix_mult(A,B); 
    clock_t end = clock();       
    printf("Time taken: %f\size",(double)(end - start)/CLOCKS_PER_SEC);

    // print_mat(A);
    // print_mat(B);
    // print_mat(C);
    
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
        
    return 0;
    
 }

int** create_mat(){
    int** mat;
    srand(time(0));
    mat = (int**)malloc(size*sizeof(int*));
    for(int j=0;j<size;j++)
       mat[j] = (int*)malloc(size*sizeof(int));
   
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            mat[i][j] = rand() % 5;
    
    return mat;
}

int** matrix_mult(int** A, int** B){
    int** C = (int**)malloc(size*sizeof(int*));
    for(int j=0;j<size;j++)
       C[j] = (int*)malloc(size*sizeof(int));
    
    for(int i=0;i<size;++i)
        for(int j=0;j<size;++j){
            C[i][j] = 0;
            for(int k=0;k<size;++k)
                C[i][j] += A[i][k]*B[k][j];
        }
    return C;
}

void free_matrix(int **mat){
    for(int i=0;i<size;i++)
        free(mat[i]);
    free(mat);
}

// void print_mat(int **mat){
//     for(int i=0;i<size;i++){
//         printf("\size");
//         for(int j=0;j<size;j++){
//             printf("%d ",mat[i][j]);
//         }
//     }
//     printf("\size");
// }