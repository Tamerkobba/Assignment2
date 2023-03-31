#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX_THREADS 16

int M, N;
int **A, **B, **C;
pthread_t threads[MAX_THREADS];
int thread_ids[MAX_THREADS];
int num_threads;

void *multiply(void *thread_id) {
    int tid = *((int *) thread_id);
    int i, j, k;
    int start_row = tid * (M / num_threads);
    int end_row = (tid + 1) * (M / num_threads);

    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < M; j++) {
            C[i][j] = 0;
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    int i, j;
    clock_t start, end;
    double cpu_time_used;

    // Prompt the user to enter M and N
    printf("Enter the number of rows (M) and columns (N) of the matrices:\n");
    scanf("%d%d", &M, &N);

    // Allocate memory for the matrices
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

    // Prompt the user to enter the number of threads to use
    printf("Enter the number of threads to use:\n");
    scanf("%d", &num_threads);

    // Perform the matrix multiplication and time it
    start = clock();
    for (i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, multiply, (void *)&thread_ids[i]);
    }
    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
   }
end = clock();
cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

// Print the result matrix C
printf("Matrix C:\n");
for (i = 0; i < M; i++) {
    for (j = 0; j < M; j++) {
        printf("%d ", C[i][j]);
    }
    printf("\n");
}

// Print the CPU time used
printf("CPU time used: %f seconds\n", cpu_time_used);

// Free the allocated memory
for (i = 0; i < M; i++) {
    free(A[i]);
    free(C[i]);
}
for (i = 0; i < N; i++) {
    free(B[i]);
}
free(A);
free(B);
free(C);

return 0;
}