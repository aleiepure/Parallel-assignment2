/**
 * Introduction to Parallel Computing (A.A. 2023/2024)
 * Homework 2: Parallelizing matrix operations using OpenMP
 *
 * Parallel matrix transposition
 *
 * Alessandro Iepure
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

float **allocateMatrix(int rows, int columns) {
    float **matrix = (float **) malloc(rows * sizeof(float *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (float *) malloc(columns * sizeof(float));
    }
    return matrix;
}

void deallocateMatrix(float **matrix, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void fill_matrix(float **m, int rows, int columns) {
    int count = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            m[i][j] = count++;
        }
    }
}

float **matT(float **M, int rows, int cols) {
    clock_t t1, t2;
    float **T = allocateMatrix(cols, rows);

    t1 = clock();
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            T[j][i] = M[i][j];
    t2 = clock();

    printf("%f, ", (t2 - t1) / 1000000.0);
    return T;
}

float **matTpar(float **M, int rows, int cols) {
    double wt1, wt2;
    float **T = allocateMatrix(cols, rows);

    wt1 = omp_get_wtime();
#pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            T[j][i] = M[i][j];
    wt2 = omp_get_wtime();

    printf("%f, ", wt2 - wt1);
    return T;
}

float **matBlockT(float **M, int rows, int cols, int b_rows, int b_cols) {
    clock_t t1, t2;
    float **T = allocateMatrix(cols, rows);

    t1 = clock();
    for (int i = 0; i < rows; i += b_rows) {
        for (int j = 0; j < cols; j += b_cols) {
            for (int x = 0; x < b_rows; x++) {
                for (int y = 0; y < b_cols; y++) {
                    T[j + y][i + x] = M[i + x][j + y];
                }
            }
        }
    }
    t2 = clock();

    printf("%f, ", (t2 - t1) / 1000000.0);
    return T;
}

float **matBlockTpar(float **M, int rows, int cols, int b_rows, int b_cols) {
    double wt1, wt2;
    float **T = allocateMatrix(cols, rows);

    wt1 = omp_get_wtime();
#pragma omp parallel for collapse(4)
    for (int i = 0; i < rows; i += b_rows) {
        for (int j = 0; j < cols; j += b_cols) {
            for (int x = 0; x < b_rows; x++) {
                for (int y = 0; y < b_cols; y++) {
                    T[j + y][i + x] = M[i + x][j + y];
                }
            }
        }
    }
    wt2 = omp_get_wtime();

    printf("%f\n", wt2 - wt1);
    return T;
}

int main(int argc, char *argv[]) {

    int rows, cols, b_rows, b_cols;

    // Handle arguments
    if (argc != 5) {
        fprintf(stderr, "Error - not enough arguments\n"
                        "Usage: %s <Row size> <Column size> <Block row size> <Block column size>\n", argv[0]);
        return -1;
    }

    if (sscanf(argv[1], "%i", &rows) != 1) {
        fprintf(stderr, "Error - <Row size> not an integer\n"
                        "Usage: %s <Row size> <Column size> <Block row size> <Block column size>\n", argv[0]);
        return -1;
    }
    if (sscanf(argv[2], "%i", &cols) != 1) {
        fprintf(stderr, "Error - <Column size> not an integer\n"
                        "Usage: %s <Row size> <Column size> <Block row size> <Block column size>\n", argv[0]);
        return -1;
    }
    if (sscanf(argv[3], "%i", &b_rows) != 1) {
        fprintf(stderr, "Error - <Block row size> not an integer\n"
                        "Usage: %s <Row size> <Column size> <Block row size> <Block column size>\n", argv[0]);
        return -1;
    }
    if (sscanf(argv[4], "%i", &b_cols) != 1) {
        fprintf(stderr, "Error - <Block column size> not an integer\n"
                        "Usage: %s <Row size> <Column size> <Block row size> <Block column size>\n", argv[0]);
        return -1;
    }

    // Test if matrix is divisible by blocks
    if (rows % b_rows != 0 || cols % b_cols != 0) {
        fprintf(stderr, "Error - Matrix %ix%i is not divisible in blocks %ix%i\n", rows, cols, b_rows,
                b_cols);
        return -1;
    }

    // Allocate memory and fill
    float **A = allocateMatrix(rows, cols);
    fill_matrix(A, rows, cols);

    // Perform transposition
    deallocateMatrix(matT(A, rows, cols), rows, cols);
    deallocateMatrix(matBlockT(A, rows, cols, b_rows, b_cols), rows, cols);
    deallocateMatrix(matTpar(A, rows, cols), rows, cols);
    deallocateMatrix(matBlockTpar(A, rows, cols, b_rows, b_cols), rows, cols);

    // Cleanup
    deallocateMatrix(A, rows, cols);
    return 0;
}