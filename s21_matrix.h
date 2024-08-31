#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define EPS (1.0e-6)
typedef enum { OK, INCORRECT_MATRIX, CALCULATION_ERROR } err_t;
typedef enum { FAILURE, SUCCESS } cmp_result;

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

err_t s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);

bool s21_eq_matrix(matrix_t *A, matrix_t *B);

err_t s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
err_t s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

err_t s21_mult_number(matrix_t *A, double number, matrix_t *result);
err_t s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

err_t s21_transpose(matrix_t *A, matrix_t *result);
err_t s21_calc_complements(matrix_t *A, matrix_t *result);
err_t s21_determinant(matrix_t *A, double *result);
err_t s21_inverse_matrix(matrix_t *A, matrix_t *result);

void print_matrix(matrix_t *matrix);

#endif  // MATRIX_H
