#include "s21_matrix.h"

static err_t create_2d_double_array(double ***array, int rows, int columns) {
  *array = calloc(rows, sizeof(double *));
  if (*array == NULL) {
    return INCORRECT_MATRIX;
  }

  for (int i = 0; i < rows; ++i) {
    (*array)[i] = calloc(columns, sizeof(double));
    if ((*array)[i] == NULL) {
      return INCORRECT_MATRIX;
    }
  }
  return OK;
}

static void destroy_2d_array(void **array, int rows) {
  if (array == NULL) return;

  for (int i = 0; i < rows; ++i) {
    if (array[i] != NULL) {
      free(array[i]);
    }
  }
  free(array);
}

err_t s21_create_matrix(int rows, int columns, matrix_t *result) {
  err_t err_code = OK;

  if (rows > 0 && columns > 0) {
    result->rows = rows;
    result->columns = columns;
  } else {
    err_code = INCORRECT_MATRIX;
  }

  if (err_code == OK) {
    err_code = create_2d_double_array(&(result->matrix), rows, columns);
  }

  return err_code;
}

void s21_remove_matrix(matrix_t *A) {
  if (A == NULL) {
    return;
  }

  destroy_2d_array((void **)A->matrix, A->rows);
  A->rows = 0;
  A->columns = 0;
  A->matrix = NULL;
}
