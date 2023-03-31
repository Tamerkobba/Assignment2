#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main() {
    int M, N, i, j, k;
    clock_t start, end;
    double cpu_time_used;

    // Prompt the user to enter M and N
    printf("Enter the number of rows (M) and columns (N) of the matrices:\n");
    scanf("%d%d", &M, &N);

    // Allocate memory for the matrices
    int **A, **B, **C;
    A = (int **)malloc(M * sizeof(int *));
    B = (int **)malloc(N * sizeof(int *));
    C = (int **)malloc(M * sizeof(int *));
    for (i = 0; i < M; i++) {
        A[i] = (int *)malloc(N * sizeof(int));
        C[i] = (int *)malloc(M * sizeof(int));
    }
    for (i = 0; i < N; i++) {
        B[i] = (int *)malloc(M * sizeof(int));
    }

    // Generate random elements for matrix A
    srand(time(NULL));
    printf("Matrix A:\n");
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = rand() % 10; // Generates a random integer between 0 and 9
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    // Generate random elements for matrix B
    printf("Matrix B:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            B[i][j] = rand() % 10; // Generates a random integer between 0 and 9
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    // Perform the matrix multiplication and time it
    start = clock();
    #pragma omp parallel for shared(A,B,C) private(i,j,k)
    for (i = 0; i < M; i++) {
        for (j = 0; j < M; j++) {
            C[i][j] = 0;
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Print the result and the time taken to execute
    printf("Resultant Matrix C:\n");
    for (i = 0; i < M; i++) {
        for (j = 0; j < M; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    printf("Time taken to execute: %f seconds\n", cpu_time_used);

    // Free the allocated memory
    for (i = 0; i < M; i++) {
        free(A[i]);
        free(C[i]);
    }
    for(i = 0; i < N; i++) {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C);
    return 0;
}