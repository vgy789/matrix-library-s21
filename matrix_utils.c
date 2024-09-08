#include "s21_matrix.h"

err_t is_valid_matrix(matrix_t *A) {
  return A != NULL && A->matrix != NULL && A->rows > 0 && A->columns > 0
             ? OK
             : INCORRECT_MATRIX;
}

err_t is_square_matrix(matrix_t *A) {
  return A->rows == A->columns ? OK : INCORRECT_MATRIX;
}

bool s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (A->columns != B->columns || A->rows != B->rows) {
    return FAILURE;
  }

  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->columns; ++j) {
      if (A->matrix[i][j] - B->matrix[i][j] > EPS) {
        return FAILURE;
      }
    }
  }
  return SUCCESS;
}

void print_matrix(matrix_t *matrix) {
  for (int i = 0; i < matrix->rows; ++i) {
    if (i != 0) printf("\n");
    for (int j = 0; j < matrix->columns; ++j) {
      if (j != 0) printf(" ");
      printf("%lf", matrix->matrix[i][j]);
    }
  }
}
