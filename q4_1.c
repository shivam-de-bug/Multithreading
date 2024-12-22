/* Implement a program in C that performs matrix multiplication using multiple threads. The
  goal here is to optimize the multiplication process by dividing the work among several
  threads.
  Input :
  - Matrix A (size: m x n)
  - Matrix B (size: n x p)
  - Resultant matrix C (m x p)
  Design :
  - Create a suitable number of threads based on the size of the resultant matrix
  allowing each thread to calculate one element of Matrix C.
  - The matrices must be allocated dynamically based on the user input
  Output :
  - Resultant matrix C along with speed up over sequential version.*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// dimensions
int m, n, p;

// matrices
int **A, **B, **C;

// calculate an element 
void *calculate_element(void *arg) {
    int i = ((int *)arg)[0];
    int j = ((int *)arg)[1];
    int sum = 0;
    
    int k = 0;
    while (k < n) {
        sum += A[i][k] * B[k][j];
        k++;
    }

    C[i][j] = sum;
    free(arg); 
    return NULL;
}

int main() {
    srand(time(NULL));

    printf("Enter number of rows for Matrix A (m): ");
    scanf("%d", &m);
    printf("Enter number of columns for Matrix A / rows for Matrix B (n): ");
    scanf("%d", &n);
    printf("Enter number of columns for Matrix B (p): ");
    scanf("%d", &p);

    A = (int **)malloc(m * sizeof(int *));
    B = (int **)malloc(n * sizeof(int *));
    C = (int **)malloc(m * sizeof(int *));
    
    int i = 0;
    while (i < m) {
        A[i] = (int *)malloc(n * sizeof(int));
        C[i] = (int *)malloc(p * sizeof(int));
        i++;
    }

    i = 0;
    while (i < n) {
        B[i] = (int *)malloc(p * sizeof(int));
        i++;
    }

    
    printf("\nEnter elements for Matrix A (size %dx%d):\n", m, n);
    i = 0;
    while (i < m) {
        int j = 0;
        while (j < n) {
            printf("A[%d][%d]: ", i, j);
            scanf("%d", &A[i][j]);
            j++;
        }
        i++;
    }

    printf("\nEnter elements for Matrix B (size %dx%d):\n", n, p);
    i = 0;
    while (i < n) {
        int j = 0;
        while (j < p) {
            printf("B[%d][%d]: ", i, j);
            scanf("%d", &B[i][j]);
            j++;
        }
        i++;
    }

    // sequential multiplication
    clock_t begin = clock();
    i = 0;
    while (i < m) {
        int j = 0;
        while (j < p) {
            C[i][j] = 0;
            int k = 0;
            while (k < n) {
                C[i][j] += A[i][k] * B[k][j];
                k++;
            }
            j++;
        }
        i++;
    }
    clock_t end = clock();
    double seq_time = ((double)(end - begin)) / CLOCKS_PER_SEC;

    // multithreaded multiplication
    begin = clock();
    pthread_t threads[m * p];
    int thread_count = 0;

    i = 0;
    while (i < m) {
        int j = 0;
        while (j < p) {
            int *arg = (int *)malloc(2 * sizeof(int));
            arg[0] = i;
            arg[1] = j;

            pthread_create(&threads[thread_count++], NULL, calculate_element, (void *)arg);
            j++;
        }
        i++;
    }

    i = 0;
    while (i < m * p) {
        pthread_join(threads[i], NULL);
        i++;
    }

    end = clock();
    double thread_time = ((double)(end - begin)) / CLOCKS_PER_SEC;

    printf("\nMatrix Multiplication Result:\n");
    i = 0;
    while (i < m) {
        int j = 0;
        while (j < p) {
            printf("%d ", C[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
    printf("\nTime taken for sequential multiplication: %f seconds\n", seq_time);

    printf("Time taken for multithreaded multiplication: %f seconds\n", thread_time);

    printf("\nSpeed-up: %f\n", seq_time / thread_time);

    // free allocated memory
    i = 0;
    while (i < m) {
        free(A[i]);
        free(C[i]);
        i++;
    }

    i = 0;
    while (i < n) {
        free(B[i]);
        i++;
    }

    free(A);
    free(B);
    free(C);

    return 0;
}
