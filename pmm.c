/*
Parallel Matrix Multiplication
This program is developed for CS3103 project by group 6.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t lock_i;        // mutex lock global variable rowcount
int *A, *B, *C;                // 1D array for matrix A, B, and C
int rowcount = 0, n, m, p = 0; // variables for matrix multiplication
/*
function mutli(void *arg)
will read global variable rowcount for locating target row
calculate all result the same row
*/
void *multi(void *arg)
{
    pthread_mutex_lock(&lock_i);
    // recieve row number of result in matrix C
    int i = rowcount++; 
    pthread_mutex_unlock(&lock_i);

    // matrix multiplication in 1D array
    if (i >= n)
        return NULL;
    for (int j = 0; j < m; j++)
        for (int k = 0; k < p; k++)
            C[i * m + j] += A[i * p + k] * B[k * m + j];

    return NULL;
}

int main(int argc, char *argv[])
{
    // get the numbers of size of source matrix from test cases
    scanf("%d%d%d", &n, &p, &m);

    // create 1D arrays to store the input
    A = (int *)malloc(n * p * sizeof(int));
    B = (int *)malloc(p * m * sizeof(int));
    C = (int *)malloc(n * m * sizeof(int));

    int i, j;
    // read matrix A from input
    for (i = 0; i < n; i++) 
        for (j = 0; j < p; j++)
            scanf("%d ", &A[i * p + j]);

    // read matrix B from input
    for (i = 0; i < p; i++) 
        for (j = 0; j < m; j++)
            scanf("%d ", &B[i * m + j]);

    // create thread pool, 8 threads has best performance
    pthread_t threads[8];
    while (rowcount < n)
    {
        for (int i = 0; i < 8; i++)
        {
            pthread_create(&threads[i], NULL, multi, NULL); // create new multi process to threads[i]
        }
        for (int i = 0; i < 8; i++)
            pthread_join(threads[i], NULL); // collect and release threads
    }

    // output the result of matrix C
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            printf("%d ", C[i * m + j]);
    printf("\n");

    // free the dynamically allocated memory
    free(A);
    free(B);
    free(C);

    return 0;
}
