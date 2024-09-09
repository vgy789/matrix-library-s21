#include "s21_matrix.h"

typedef double (*arithmetic_func)(double, double);

static double add(double num_1, double num_2) { return num_1 + num_2; }
static double sub(double num_1, double num_2) { return num_1 - num_2; }

static err_t sumsub_matrix(matrix_t *A, matrix_t *B, matrix_t *result,
                           const arithmetic_func operation) {
  err_t err_code = OK;

  err_code =
      (is_valid_matrix(A) == OK && is_valid_matrix(B) == OK ? OK
                                                            : INCORRECT_MATRIX);

  if (err_code == OK) {
    const bool is_equal_size = (A->rows == B->rows && A->columns == B->columns);
    err_code = (is_equal_size ? OK : CALCULATION_ERROR);
  }

  if (err_code == OK) {
    err_code = s21_create_matrix(A->rows, B->columns, result);
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
  for (int i = 0; i < result->rows; ++i) {
    for (int j = 0; j < result->columns; ++j) {
      for (int k = 0; k < A->columns; ++k) {
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
    err_code = (A->columns == B->rows ? OK : CALCULATION_ERROR);
  }

  if (err_code == OK) {
    err_code = s21_create_matrix(A->rows, B->columns, result);
  }

  if (err_code == OK) {
    mult_matrix(A, B, result);
  }

  return err_code;
}

static void get_submatrix(matrix_t *A, matrix_t *submatrix, int row,
                          int column) {
  bool row_passed = false;
  bool column_passed = false;

  for (int i = 0; i < A->rows; ++i) {
    if (i == row) continue;
    row_passed = (i > row);
    for (int j = 0; j < A->columns; ++j) {
      if (j == column) continue;
      column_passed = (j > column);
      submatrix->matrix[i - row_passed][j - column_passed] = A->matrix[i][j];
    }
  }
}

static double det_recursive(matrix_t *A) {
  double det = 0.0;
  if (A->rows == 1) {
    return A->matrix[0][0];
  }
  if (A->rows == 2) {
    return A->matrix[0][0] * A->matrix[1][1] -
           A->matrix[0][1] * A->matrix[1][0];
  }

  matrix_t submatrix;
  (void)s21_create_matrix(A->rows - 1, A->rows - 1, &submatrix);

  // https://ru.onlinemschool.com/math/library/matrix/minors/#h2
  // разложение по первой строке
  int sign = +1;
  for (int k = 0; k < A->rows; ++k) {
    get_submatrix(A, &submatrix, 0, k);
    det += sign * A->matrix[0][k] * det_recursive(&submatrix);
    sign = -sign;
  }
  s21_remove_matrix(&submatrix);

  return det;
}

err_t s21_determinant(matrix_t *A, double *result) {
  err_t err_code = OK;

  err_code = is_valid_matrix(A);
  if (err_code == OK) {
    err_code = is_square_matrix(A);
  }
  if (err_code == OK) {
    *result = det_recursive(A);
  }
  return err_code;
}

err_t s21_transpose(matrix_t *A, matrix_t *result) {
  err_t err_code = OK;

  err_code = is_valid_matrix(A);
  if (err_code == OK) {
    err_code = s21_create_matrix(A->columns, A->rows, result);
  }
  if (err_code == OK) {
    for (int i = 0; i < A->rows; ++i) {
      for (int j = 0; j < A->columns; ++j) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }

  return err_code;
}

static void get_minor_matrix(matrix_t *A, matrix_t *M) {
  matrix_t submatrix;
  (void)s21_create_matrix(A->rows - 1, A->rows - 1, &submatrix);
  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->columns; ++j) {
      get_submatrix(A, &submatrix, i, j);
      M->matrix[i][j] = det_recursive(&submatrix);
    }
  }

  s21_remove_matrix(&submatrix);
}

err_t s21_calc_complements(matrix_t *A, matrix_t *result) {
  err_t err_code = OK;

  err_code = is_valid_matrix(A);
  if (err_code == OK) {
    err_code = is_square_matrix(A);
  }

  if (err_code == OK) {
    err_code = s21_create_matrix(A->rows, A->rows, result);
  }
  if (err_code == OK) {
    get_minor_matrix(A, result);
    for (int i = 0; i < A->rows; ++i) {
      for (int j = 0; j < A->columns; ++j) {
        int sign = pow(-1, i + j);
        result->matrix[i][j] = sign * result->matrix[i][j];
      }
    }
  }

  return err_code;
}

err_t s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  err_t err_code = OK;
  double det = 0.;

  err_code = is_valid_matrix(A);
  if (err_code == OK) {
    err_code = is_square_matrix(A);
  }
  if (err_code == OK) {
    det = det_recursive(A);
    err_code = (det != 0 ? OK : CALCULATION_ERROR);
  }

  if (err_code == OK) {
    matrix_t M;
    s21_calc_complements(A, &M);
    matrix_t MT;
    s21_transpose(&M, &MT);
    s21_mult_number(&MT, 1.0 / det, result);
    s21_remove_matrix(&M);
    s21_remove_matrix(&MT);
  }

  return err_code;
}
