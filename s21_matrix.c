#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int check = 0;
  result->columns = columns;
  result->rows = rows;
  if ((rows > 0) || (columns > 0)) {
    result->matrix = (double **)calloc(rows, sizeof(double));
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = (double *)calloc(columns, sizeof(double));
    }
  } else {
    check = 1;
  }
  return check;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    A->rows = 0;
    A->columns = 0;
    free(A->matrix);
    A->matrix = NULL;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int eq = SUCCESS;
  if (A == NULL || A->rows < 1 || B == NULL || B->rows < 1) {
    eq = FAILURE;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    eq = FAILURE;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 0.0000001) {
          eq = FAILURE;
        }
      }
    }
  }
  return eq;
}
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int check = 0;
  if ((A->rows == B->rows) && (A->columns == B->columns)) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  } else {
    check = 2;
  }
  return check;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int check = 0;
  if ((A->rows == B->rows) && (A->columns == B->columns)) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  } else {
    check = 2;
  }
  return check;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int check = 0;
  if (A == NULL || A->rows < 1 || A->columns < 1) {
    check = 1;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }
  return check;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int check = 0;
  if ((A == NULL || A->rows < 1 || A->columns < 1) &&
      (B == NULL || B->rows < 1 || B->columns < 1)) {
    check = 1;
  } else if (A->columns != B->rows) {
    check = 2;
  } else {
    s21_create_matrix(A->rows, B->columns, result);
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        for (int k = 0; k < A->columns; k++) {
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
  }
  return check;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int check = 0;
  if (A == NULL || A->rows < 1 || A->columns < 1) {
    check = 1;
  } else {
    s21_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }
  return check;
}

void s21_min(int k, int p, matrix_t *A, matrix_t *matx) {
  int su_i = 0;
  for (int i = 0; i < A->rows; i++) {
    int su_j = 0;
    if (i != k) {
      for (int j = 0; j < A->columns; j++) {
        if (j != p) {
          matx->matrix[su_i][su_j] = A->matrix[i][j];
          su_j++;
        }
      }
      su_i++;
    }
  }
}

int s21_determinant(matrix_t *A, double *result) {
  int check = 0;
  if ((A->rows > 0) && (A->columns > 0)) {
    if (A->rows == A->columns) {
      if (A->rows == 1) {
        *result = A->matrix[0][0];
      } else {
        *result = s21_det(A->rows, A);
      }
    } else {
      check = 2;
    }
  } else {
    check = 1;
  }
  return check;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int check = 0;
  if ((A->rows > 0) && (A->columns > 0)) {
    if (A->rows == A->columns) {
      s21_create_matrix(A->rows, A->columns, result);
      matrix_t mat;
      s21_create_matrix(A->rows - 1, A->columns - 1, &mat);
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          s21_min(i, j, A, &mat);
          s21_determinant(&mat, &result->matrix[i][j]);
          result->matrix[i][j] *= pow(-1, i + j);
        }
      }
      s21_remove_matrix(&mat);
    } else {
      check = 2;
    }
  } else {
    check = 1;
  }
  return check;
}

double s21_det(int n, matrix_t *A) {
  matrix_t mat;
  s21_create_matrix(A->rows, A->columns, &mat);
  double check = 0;
  if (n == 2) {
    check =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[1][0] * A->matrix[0][1];
  } else {
    for (int c = 0; c < n; c++) {
      int su_i = 0;
      for (int i = 1; i < n; i++) {
        int su_j = 0;
        for (int j = 0; j < n; j++) {
          if (j != c) {
            mat.matrix[su_i][su_j] = A->matrix[i][j];
            su_j++;
          }
        }
        su_i++;
      }
      check += (pow(-1, c) * A->matrix[0][c] * s21_det(n - 1, &mat));
    }
  }
  s21_remove_matrix(&mat);
  return check;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int check = 0;
  if ((A->rows > 0) && (A->columns > 0)) {
    if (A->rows == A->columns) {
      double check2 = 0;
      s21_determinant(A, &check2);
      if (fabs(check2) >= 0.0000001) {
        matrix_t a_2;
        matrix_t tp_2;
        s21_calc_complements(A, &a_2);
        s21_transpose(&a_2, &tp_2);
        s21_mult_number(&tp_2, pow(check2, -1), result);
        s21_remove_matrix(&a_2);
        s21_remove_matrix(&tp_2);
      } else {
        check = 2;
      }
    } else {
      check = 2;
    }
  } else {
    check = 1;
  }
  return check;
}