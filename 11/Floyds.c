#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

// defining infinity
#define INF 99999 
// number of Nodes
#define N 4 

void printSolution(int dist[][N]);

void floyd(int graph[][N]){

    int dist[N][N],i,j,k;
    // dist has the final solution
    for(i = 0 ; i < N ;i++){
        for(j = 0 ; j < N ;j++){
            dist[i][j] = graph[i][j];
        }
    }
    double start = omp_get_wtime();

    for(k = 0; k < N ; k++){
        for(i = 0 ; i < N ; i++){
            for(j = 0 ; j < N ; j++){

                if(dist[i][k] + dist[k][j] < dist[i][j]){
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    double end = omp_get_wtime();

    printf("Elapsed time %f\n", end-start);

    printSolution(dist);

}

void printSolution(int dist[][N]){

    printf("Adjacency Matrix after Floyd-Warshall Algorithm.\n");
    
    for(int  i = 0 ; i < N ; i++){
        for(int j = 0 ; j < N ; j++){

            if(dist[i][j] == INF){
                printf("%7s" , "INF");
            }
            else printf("%7d" , dist[i][j]);
        }

        printf("\n");
    }
}


int main(){

    // int graph[N][N] = { { 0, 5, INF, 10, 2 },
    //                     { INF, 0, 3, INF, 3 },
    //                     { INF, INF, 0, 1, INF },
    //                     { INF, INF, INF, 0 ,1} };
    
     int graph[N][N] = {0, INF, -2, INF,
                             4, 0, 3, INF, 
                             INF, INF, 0, 2,
                             INF, -1, INF, 0};

    floyd(graph);
    return 0;
}