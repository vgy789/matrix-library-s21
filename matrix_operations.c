#include "s21_matrix.h"

typedef double (*arithmetic_func)(double, double);

static double add(double num_1, double num_2) { return num_1 + num_2; }
static double sub(double num_1, double num_2) { return num_1 - num_2; }

// функции sum и sub имеют одинаковый профиль (одинаковые заголовки)
static err_t sumsub_matrix(matrix_t *A, matrix_t *B, matrix_t *result,
                           const arithmetic_func operation) {
  err_t err_code = OK;

  err_code =
      (is_valid_matrix(A) == OK && is_valid_matrix(B) == OK ? OK
                                                            : INCORRECT_MATRIX);

  if (err_code == OK) {
    const bool is_equal_size = (A->rows == B->rows && A->columns == B->columns);
    err_code = (is_equal_size ? OK : INCORRECT_MATRIX);
  }

  if (err_code == OK) {
    err_code = s21_create_matrix(A->rows, B->columns, result);
  }
  if (err_code == OK) {
    err_code = s21_create_matrix(A->rows, A->columns, result);
  }

  if (err_code == OK) {
    for (int i = 0; i < A->rows; ++i) {
      for (int j = 0; j < A->columns; ++j) {
        result->matrix[i][j] = operation(A->matrix[i][j], B->matrix[i][j]);
      }
    }
  }

  return err_code;
}

err_t s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  const err_t err_code = sumsub_matrix(A, B, result, add);
  return err_code;
}

err_t s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  const err_t err_code = sumsub_matrix(A, B, result, sub);
  return err_code;
}

err_t s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  err_t err_code = OK;

  err_code = is_valid_matrix(A);
  if (err_code == OK) {
    err_code = s21_create_matrix(A->columns, A->rows, result);
  }
  if (err_code == OK) {
    for (int i = 0; i < A->rows; ++i) {
      for (int j = 0; j < A->columns; ++j) {
        result->matrix[i][j] = (A->matrix[i][j] * number);
      }
    }
  }

  return err_code;
}

static void mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < B->columns; ++j) {
      for (int k = 0; k < B->rows; ++k) {
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }
}

err_t s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  err_t err_code = OK;

  err_code =
      (is_valid_matrix(A) == OK && is_valid_matrix(B) == OK ? OK
                                                            : INCORRECT_MATRIX);
  if (err_code == OK) {
    err_code = (A->columns == B->rows ? OK : INCORRECT_MATRIX);
  }

  if (err_code == OK) {
    err_code = s21_create_matrix(A->rows, B->columns, result);
  }

  if (err_code == OK) {
    mult_matrix(A, B, result);
  }

  return err_code;
}

// s21_minor_matrix(matrix_t *A, matrix_t *result) {
//   err_t err_code = OK;

//   err_code = is_valid_matrix(A);
//   if (err_code == OK && (A->columns != A->rows)) {
//     err_code = CALCULATION_ERROR;
//   }
//   if (err_code == OK) {
//     err_code = s21_create_matrix(A->rows, A->columns, result);
//   }
//   return err_code;
// }

// err_t s21_calc_complements(matrix_t *A, matrix_t *result) {

// }

err_t s21_determinant(matrix_t *A, double *result) {
  err_t err_code = OK;

  if (is_square_matrix(A) != OK || result == NULL) {
    return INCORRECT_MATRIX;
  }

  if (A->rows == 2) {
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    return OK;
  }

  if (A->rows == 1) {
    *result = A->matrix[0][0];
    return OK;
  }

  // https://ru.onlinemschool.com/math/library/matrix/minors/#h2
  // разложение по строке
  matrix_t minor;
  s21_create_matrix(A->rows - 1, A->rows - 1, &minor);
  *result = 0;

  for (int k = 0; k < A->columns; ++k) {
    for (int i = 1; i < A->rows; ++i) {
      for (int j = 0; j < A->columns; ++j) {
        if (j == k) {
          continue;
        }
        minor.matrix[i - 1][j - (j > k)] = A->matrix[i][j];
      }
    }
    *result += A->matrix[0][k] * (pow(-1.f, k)) *
               (minor.matrix[0][0] * minor.matrix[1][1] -
                minor.matrix[0][1] * minor.matrix[1][0]);
  }

  if (A->rows > 3) {
  }

  s21_remove_matrix(&minor);
  return err_code;
}

err_t s21_transpose(matrix_t *A, matrix_t *result) {
  err_t err_code = OK;

  err_code = s21_create_matrix(A->columns, A->rows, result);
  if (err_code == OK) {
    for (int i = 0; i < A->rows; ++i) {
      for (int j = 0; j < A->columns; ++j) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }

  return err_code;
}

// err_t s21_inverse_matrix(matrix_t *A, matrix_t *result) {

// }
