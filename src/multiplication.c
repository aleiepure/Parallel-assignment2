/**
 * Introduction to Parallel Computing (A.A. 2023/2024)
 * Homework 2: Parallelizing matrix operations using OpenMP
 *
 * Parallel matrix multiplication
 *
 * Alessandro Iepure
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
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

void fill_matrix(float **m, int rows, int columns, bool is_dense) {
    if (is_dense) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                m[i][j] = (i + j) * 0.5;
            }
        }
    } else {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                m[i][j] = (i + j) * 0.5;
                if ((j % 5 != 0 || i % 8 == 0) && (j % 4 == 0 || i % 3 != 0))
                    m[i][j] = 0;
            }
        }
    }
}

float **matMul(float **A, float **B, int a_rows, int a_columns, int b_rows, int b_columns) {
    clock_t t1, t2;
    float **C = allocateMatrix(a_rows, b_columns);

    t1 = clock();
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_columns; j++) {
            C[i][j] = 0;
            for (int k = 0; k < a_columns; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    }
    t2 = clock();

    printf("%f, ", (t2 - t1) / 1000000.0);
    return C;
}

float **matMulPar(float **A, float **B, int a_rows, int a_columns, int b_rows, int b_columns) {
    double wt1, wt2;
    float temp;
    float **C = allocateMatrix(a_rows, b_columns);

    wt1 = omp_get_wtime();

#pragma omp parallel for collapse(2) reduction(+:temp)
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_columns; j++) {
            temp = 0;
            for (int k = 0; k < a_columns; k++) {
                temp += A[i][k] * B[k][j];
            }
            C[i][j] = temp;
        }
    }

    wt2 = omp_get_wtime();

    printf("%f\n", wt2 - wt1);
    return C;
}

int main(int argc, char *argv[]) {

    bool is_dense;
    int a_rows, a_columns, b_rows, b_columns;

    // Handle arguments
    if (argc != 6) {
        fprintf(stderr, "Error - not enough arguments\n"
                        "Usage: %s <dense|sparse> <A rows> <A columns> <B rows> <B columns> <block rows> <block columns>\n",
                argv[0]);
        return -1;
    }

    if (strcmp(argv[1], "dense") == 0) {
        is_dense = true;
    } else if (strcmp(argv[1], "sparse") == 0) {
        is_dense = false;
    } else {
        fprintf(stderr, "Error - <dense|parse> not correct\n"
                        "Usage: %s <dense|sparse> <A rows> <A columns> <B rows> <B columns> <block rows> <block columns>\n",
                argv[0]);
        return -1;
    }

    if (sscanf(argv[2], "%i", &a_rows) != 1) {
        fprintf(stderr, "Error - <A rows> not an integer\n"
                        "Usage: %s <dense|sparse> <A rows> <A columns> <B rows> <B columns> <block rows> <block columns>\n",
                argv[0]);
        return -1;
    }
    if (sscanf(argv[3], "%i", &a_columns) != 1) {
        fprintf(stderr, "Error - <A columns> not an integer\n"
                        "Usage: %s <dense|sparse> <A rows> <A columns> <B rows> <B columns> <block rows> <block columns>\n",
                argv[0]);
        return -1;
    }
    if (sscanf(argv[4], "%i", &b_rows) != 1) {
        fprintf(stderr, "Error - <B rows> not an integer\n"
                        "Usage: %s <dense|sparse> <A rows> <A columns> <B rows> <B columns> <block rows> <block columns>\n",
                argv[0]);
        return -1;
    }
    if (sscanf(argv[5], "%i", &b_columns) != 1) {
        fprintf(stderr, "Error - <B columns> not an integer\n"
                        "Usage: %s <dense|sparse> <A rows> <A columns> <B rows> <B columns> <block rows> <block columns>\n",
                argv[0]);
        return -1;
    }

    // Check compatibility
    if (a_columns != b_rows) {
        fprintf(stderr, "Error - Incompatible matrices for multiplication\n");
        return -1;
    }

    // Allocate memory and fill
    float **A = allocateMatrix(a_rows, a_columns);
    float **B = allocateMatrix(b_rows, b_columns);

    fill_matrix(A, a_rows, a_columns, is_dense);
    fill_matrix(B, b_rows, b_columns, is_dense);

    // Perform multiplication
    deallocateMatrix(matMul(A, B, a_rows, a_columns, b_rows, b_columns), a_rows, b_columns);
    deallocateMatrix(matMulPar(A, B, a_rows, a_columns, b_rows, b_columns), a_rows, b_columns);

    // Clean up
    deallocateMatrix(A, a_rows, a_columns);
    deallocateMatrix(B, b_rows, b_columns);
    return 0;
}