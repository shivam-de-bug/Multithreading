/*In the previous question, you created a separate thread for computing each element of
  Matrix C which can hamper the performance if the size of Matrix C is large. Therefore in
  this part of the question, you are required to implement a thread pool that will reuse
  threads for computing each element of Matrix C.
  Design :
  - Implement a thread pool that can manage a fixed number of threads using the
  pthreads API (the number of threads allocated must be equal to the number of
  cores in your system).

  - Each thread in the pool should be capable of performing a single task to compute
  one element of the resultant matrix C.
  - Suggestion: Instead of hardcoding the thread pool API to work with matrix
  multiplication, you can also design it as a generic API, that accepts a callback
  with some data as input and runs the callback with data.

  Input: Same as part 1(q4_1)
  Output: Resultant matrix C along with speed up over sequential version and part 1 parallel
  implementation.*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// dimensions
int m, n, p;

// matrix
int **A, **B, **C;

// thread pool 
pthread_t *t_pool;
int n_threads;
int ind= 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//  calculate 
void *calc_elem(void *arg) {
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

//  thread pool matrix multiplication 
void *mat_mul(void *arg) {
    while (1) {
        int i = -1, j = -1;

        // find unprocessed element
        pthread_mutex_lock(&mutex);
        if (ind < m * p) {
            i = ind / p;
            j = ind % p;
            ind++;
        }
        pthread_mutex_unlock(&mutex);

        if (i == -1 || j == -1) break;

        // calculation
        int sum = 0;
        int k = 0;
        while (k < n) {
            sum += A[i][k] * B[k][j];
            k++;
        }
        C[i][j] = sum;
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    printf("Enter rows for Matrix A (m): ");
    scanf("%d", &m);
    printf("Enter cols for Matrix A / rows for Matrix B (n): ");
    scanf("%d", &n);
    printf("Enter cols for Matrix B (p): ");
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

    // Sequential matrix multiplication 
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

    //  as in part 1: Multithreaded matrix multiplication 
    begin = clock();
    pthread_t thrs[m * p];
    int count = 0;
    i = 0;
    while (i < m) {
        int j = 0;
        while (j < p) {
            int *arg = (int *)malloc(2 * sizeof(int));
            arg[0] = i;
            arg[1] = j;
            pthread_create(&thrs[count++], NULL, calc_elem, (void *)arg);
            j++;
        }
        i++;
    }
    i = 0;
    while (i < m * p) {
        pthread_join(thrs[i], NULL);
        i++;
    }
    end = clock();
    double thread_time = ((double)(end - begin)) / CLOCKS_PER_SEC;

    // thread pool to perform matrix multiplication
    n_threads = sysconf(_SC_NPROCESSORS_ONLN); // cores
    t_pool = (pthread_t *)malloc(n_threads * sizeof(pthread_t));
    ind= 0; 

    i = 0;
    while (i < m) {
        int j = 0;
        while (j < p) {
            C[i][j] = 0;
            j++;
        }
        i++;
    }

    // thread pool implement
    begin= clock();
    i = 0;
    while (i < n_threads) {
        pthread_create(&t_pool[i], NULL, mat_mul, NULL);
        i++;
    }
    i = 0;
    while (i < n_threads) {
        pthread_join(t_pool[i], NULL);
        i++;
    }
    end = clock();
    double pool_time = ((double)(end - begin)) / CLOCKS_PER_SEC;

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
    printf("Time taken for multithreaded multiplication (each thread per element (part1) ): %f seconds\n", thread_time);
    printf("Time taken for multithreaded multiplication (with thread pool): %f seconds\n", pool_time);
    printf("\nSpeed-up (sequential vs. multithreaded with thread pool): %f\n", seq_time / pool_time);
    printf("\nSpeed-up (multithreaded vs. multithreaded with thread pool): %f\n", thread_time / pool_time);

    // free memory
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
    free(t_pool);

    return 0;
}
