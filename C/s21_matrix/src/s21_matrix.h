#ifndef S21_MATRIX_H
#define S21_MATRIX_H

#include <stdlib.h>
#include <math.h>

#define FAILED_MATRIX 2
#define INCORRECT_MATRIX 1
#define OK 0

#define SUCCESS 1
#define FAILURE 0

typedef struct matrix_struct {
    double** matrix;
    int rows;
    int columns;
} matrix_t;

int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
int s21_eq_matrix(matrix_t *A, matrix_t *B);

#endif