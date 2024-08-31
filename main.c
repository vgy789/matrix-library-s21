#include "s21_matrix.h"

int main(void) {
  matrix_t A = (matrix_t){0};

  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 10.f;
  A.matrix[0][1] = 2.f;
  A.matrix[0][2] = 3.f;
  A.matrix[1][0] = 4.f;
  A.matrix[1][1] = 5.f;
  A.matrix[1][2] = 6.f;
  A.matrix[2][0] = 7.f;
  A.matrix[2][1] = 8.f;
  A.matrix[2][2] = 9.f;

  // s21_create_matrix(1,1, &A);
  // A.matrix[0][0] = 1.f;

  double result = 0.f;
  err_t err_code = s21_determinant(&A, &result);
  printf("%lf", result);
}

//   if (expected_code == OK) {
//     bool is_eq = s21_eq_matrix(&result, expected);
//     ck_assert(is_eq == SUCCESS);
//     s21_remove_matrix(A);
//     s21_remove_matrix(expected);
//   }
// }
