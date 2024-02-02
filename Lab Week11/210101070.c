#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 400
#define NUM_THREADS 4

int matrix1[SIZE][SIZE];
int matrix2[SIZE][SIZE];
int result[SIZE][SIZE];

pthread_mutex_t mutex;

struct ThreadData {
    // thread data for matrix 1 and matrix 2 are based on the start and end values of column and row numbers respectively
    // startRow = startCol; endRow = endCol;
    // data of each thread[i] is m[1 to 400][(i+1)*1 to (i+1)*100] and m[(i+1)*1 to (i+1)*100][1 to 400]
    int startRow;
    int endRow;
};

void *multiply(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            int sum = 0;
            for (int k = data->startRow; k < data->endRow; ++k) {
                sum += matrix1[i][k] * matrix2[k][j];
                // sum = matrix[i][colstart to colend]*matrix2[rowstart to rowend][j]
            }
            // mutex locking to avoid changing of same data in simultaneous synchonized threads
            pthread_mutex_lock(&mutex);
            result[i][j] += sum;    // complete result matrx is updated after each thread
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}

void initializeMatrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            matrix[i][j] = rand() % 10; // Randomly initialize the matrix
        }
    }
}

void printMatrixToFile(int matrix[SIZE][SIZE], const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main() {
    srand(time(NULL));

    // Initialize matrices
    initializeMatrix(matrix1);
    initializeMatrix(matrix2);

    pthread_t threads[NUM_THREADS];
    struct ThreadData threadData[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        threadData[i].startRow = i * (SIZE / NUM_THREADS);
        threadData[i].endRow = (i + 1) * (SIZE / NUM_THREADS);

        // each thread is initialized and each part of multiplication is done and added at after each thread
        pthread_create(&threads[i], NULL, multiply, (void *)&threadData[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    // Print result matrix to a file
    printMatrixToFile(matrix1,"m1.txt");
    printMatrixToFile(matrix2,"m2.txt");
    printMatrixToFile(result,"result_matrix.txt");

    return 0;
}
