
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define P_SQRT 2 
#define P 4
#define MATRIX_SIZE 500
#define BLOCK_SIZE (MATRIX_SIZE/2)

#define TRUE 1
#define FALSE 0

struct struct_matrix {
    int nrow;
    int ncol;
    int **data;
};

typedef struct struct_matrix Matrix; 

void create_matrix(Matrix *m, int nrow, int ncol) { 
    int i;
    m->nrow = nrow; 
    m->ncol = ncol;
    m->data = malloc(nrow *  sizeof(int*));
    for(i = 0; i < ncol; i++){
        m->data[i] = calloc(ncol, sizeof(int));
    }
}

void populate_matrix(Matrix *m) {
    int i, j;
    for(i = 0; i < m->nrow; i++){
        for(j = 0; j < m->ncol; j++){
            m->data[i][j] = rand() % 10 + 1;
        }
    }
}

void print_matrix(Matrix *m, char iden) {
    int i, j;

    for(i = 0; i < m->nrow; i++){
        for(j = 0; j < m->ncol; j++){
            printf("%c[%d][%d] = %d  ", iden, i, j , m->data[i][j]);
        }
        printf("\n");
    }
}

void shift_matrix_left(Matrix *m, int block_sz, int initial) {
    int i, j, k, s, step = block_sz;
    Matrix aux;
    
    create_matrix(&aux, 1, m->ncol);
    for(k = 0, s = 0; k < m->ncol; k += block_sz, s++)
        for(i = k; i < (k + block_sz); i++){
            if(initial > 0)
                step = s * block_sz;
            for(j = 0; j < m->ncol; j++)
                aux.data[0][j] = m->data[i][(j + step) % m->ncol];
            for(j = 0; j < m->ncol; j++)
                m->data[i][j] = aux.data[0][j];
        }
}

void shift_matrix_up(Matrix *m, int block_sz, int initial) {
    int i, j, k, s, step = block_sz;
    Matrix aux;
    
    create_matrix(&aux, 1, m->nrow);
    for(k = 0, s = 0; k < m->nrow; k += block_sz, s++)
        for(i = k; i < (k + block_sz); i++){
            if(initial > 0)
                step = s * block_sz;
            for(j = 0; j < m->nrow; j++)
                aux.data[0][j] = m->data[(j + step) % m->nrow][i];
            for(j = 0; j < m->nrow; j++)
                m->data[j][i] = aux.data[0][j];
        }
}

void matrix_product(Matrix *c, Matrix *a, Matrix *b){
    int r, s, k;

    for(r = 0; r < a->nrow; r++)
        for(s = 0; s < b->ncol; s++)
            for(k = 0; k < a->ncol; k++)
                c->data[r][s] += a->data[r][k] * b->data[k][s];
}

int* create_array_as_matrix(int r, int c){
    int *mat = calloc(r * c, sizeof(int));
    return mat;
}

void populate_array_as_matrix(int *arr, int r, int c){
    int j; 
    for(j = 0; j < r*c; j++){
        arr[j] = rand() % 10 + 1;
    }
}

int array_as_matrix_equals(int *a, int *b, int r, int c){
    int i = 0;
    for(i = 0; i < r*c; i++){
        if(a[i] != b[i]){
            return FALSE;
        } 
    }
    return TRUE;
}

// Cannon's Multiplication functions:

void process_mult(Matrix *A, Matrix *B, Matrix *C) {
    int r, c, id, k, rbegin, rend, cbegin, cend,l, m;
    Matrix sa, sb, sc;

    #pragma omp parallel private(l, m, r, c, k, rbegin, rend, cbegin, cend, id, sa, sb, sc) shared(A, B, C) num_threads(P)
    {
        id = omp_get_thread_num();
        rbegin = (id / P_SQRT) * BLOCK_SIZE;
        rend = rbegin + BLOCK_SIZE;

        cbegin = (id % P_SQRT) * BLOCK_SIZE;
        cend = cbegin + BLOCK_SIZE;

        create_matrix(&sa, BLOCK_SIZE, BLOCK_SIZE);
        create_matrix(&sb, BLOCK_SIZE, BLOCK_SIZE);
        create_matrix(&sc, BLOCK_SIZE, BLOCK_SIZE);

        //copy the blocks for this process
        for(r = rbegin, l = 0; r < rend; r++, l++)
            for(c = cbegin, m = 0; c < cend; c++, m++){
                sa.data[l][m] = A->data[r][c];
                sb.data[l][m] = B->data[r][c];
                sc.data[l][m] = C->data[r][c];
            }
        
        matrix_product(&sc, &sa, &sb);

        //put results back to C
        for(r = rbegin, l = 0; r < rend; r++, l++)
            for(c = cbegin, m = 0; c < cend; c++, m++)
                C->data[r][c] = sc.data[l][m];
    }
}

int main() {
    Matrix A, B, C;

    create_matrix(&A, MATRIX_SIZE, MATRIX_SIZE);
    create_matrix(&B, MATRIX_SIZE, MATRIX_SIZE);
    create_matrix(&C, MATRIX_SIZE, MATRIX_SIZE);

    populate_matrix(&A);
    populate_matrix(&B);
    
    shift_matrix_left(&A, BLOCK_SIZE, 1);
    shift_matrix_up(&B, BLOCK_SIZE, 1);

    double t1 = omp_get_wtime();
    for(int i = 0; i < P_SQRT; i++){
        process_mult(&A, &B, &C);
        shift_matrix_left(&A, BLOCK_SIZE, 0);
        shift_matrix_up(&B, BLOCK_SIZE, 0);
    }
    double t2 = omp_get_wtime();

    printf("Time: %.4f seconds\n", (t2 - t1));
    return 0;
}
