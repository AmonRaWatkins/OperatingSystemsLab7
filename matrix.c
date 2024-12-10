#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20  // Defines the size of the matrix

// Global matrices
int matA[MAX][MAX]; 
int matB[MAX][MAX]; 
int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

// Data structure for thread arguments
struct Data {
    int start_row;
    int end_row;
};

// Function to fill a matrix with random numbers
void fillMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;  // Random values between 1 and 10
        }
    }
}

// Function to print a matrix
void printMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Compute the sum of corresponding elements of matA and matB
void* computeSum(void* args) {
    struct Data *data = (struct Data *)args;
    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < MAX; j++) {
            matSumResult[i][j] = matA[i][j] + matB[i][j];
        }
    }
    pthread_exit(0);
}

// Compute the difference of corresponding elements of matA and matB
void* computeDiff(void* args) {
    struct Data *data = (struct Data *)args;
    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < MAX; j++) {
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
        }
    }
    pthread_exit(0);
}

// Compute the product of matA and matB
void* computeProduct(void* args) {
    struct Data *data = (struct Data *)args;
    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < MAX; j++) {
            matProductResult[i][j] = 0; // Initialize to 0
            for (int k = 0; k < MAX; k++) {
                matProductResult[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    pthread_exit(0);
}

int main() {
    srand(time(0));  // Seed the random number generator
    
    // Fill the matrices with random values
    fillMatrix(matA);
    fillMatrix(matB);

    // Print the initial matrices
    printf("Matrix A:\n");
    printMatrix(matA);
    
    printf("Matrix B:\n");
    printMatrix(matB);

    // Create threads
    pthread_t threads[10];
    struct Data thread_data[10];
    
    int rows_per_thread = MAX / 10;

    // Launch threads for sum
    for (int i = 0; i < 10; i++) {
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, computeSum, &thread_data[i]);
    }
    
    // Wait for sum threads to finish
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    // Launch threads for difference
    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, computeDiff, &thread_data[i]);
    }
    
    // Wait for difference threads to finish
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    // Launch threads for product
    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, computeProduct, &thread_data[i]);
    }

    // Wait for product threads to finish
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the results
    printf("Sum of matrices:\n");
    printMatrix(matSumResult);
    
    printf("Difference of matrices:\n");
    printMatrix(matDiffResult);
    
    printf("Product of matrices:\n");
    printMatrix(matProductResult);

    return 0;
}