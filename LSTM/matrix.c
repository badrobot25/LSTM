// matrix.c
#include "matrix.h"

matrix_type *make_matrix(unsigned int rows, unsigned int columns) {
  matrix_type *matrix = NULL;
  matrix = malloc(sizeof(matrix_type));
  assert(matrix);
  matrix->rows = rows;
  matrix->columns = columns;
  matrix->matrix = malloc(sizeof(long double *) * rows);
  assert(matrix->matrix);
  for (unsigned int row = 0; row < rows; row++) {
    matrix->matrix[row] = malloc(sizeof(long double) * columns);
    assert(matrix->matrix[row]);
  } return matrix;
}

void destroy_matrix(matrix_type *matrix) {
  for (unsigned int row = 0; row < matrix->rows; row++) {
    free(matrix->matrix[row]);
    matrix->matrix[row] = NULL;
  } free(matrix->matrix);
    matrix->matrix = NULL;
    free(matrix);
    matrix = NULL;
}

long double random_long_double(long double x) {
  NOT_USED(x);
  return 2.0 * (((long double)rand()) / ((long double)RAND_MAX)) - 1.0;
}

long double zero(long double x) {NOT_USED(x); return 0.0;}

long double one(long double x) {NOT_USED(x); return 1.0;}

void matrix_for_each(long double (*f)(long double), matrix_type *matrix) {
  for (unsigned int row = 0; row < matrix->rows; row++) {
    for (unsigned int column = 0; column < matrix->columns; column++) {
      matrix->matrix[row][column] = f(matrix->matrix[row][column]);
    }
  }
}

matrix_type *dot_product(matrix_type *matrix1, matrix_type *matrix2) {
  matrix_type *matrix3 = make_matrix(matrix1->rows, matrix2->columns);
  assert(matrix1->columns == matrix2->rows);
  matrix_for_each(zero, matrix3);

  for (unsigned int row1 = 0; row1 < matrix1->rows; row1++) {
    for (unsigned int column2 = 0; column2 < matrix2->columns; column2++) {
      for (unsigned int column1 = 0; column1 < matrix1->columns; column1++) {
        matrix3->matrix[row1][column2] += matrix1->matrix[row1][column1] * matrix2->matrix[column1][column2];
      }
    }
  } destroy_matrix(matrix1);
    destroy_matrix(matrix2);
    return matrix3;
}

matrix_type *matrix_copy_shape(matrix_type *matrix1) {
  return make_matrix(matrix1->rows, matrix1->columns);
}

matrix_type *matrix_copy(matrix_type *matrix1) {
  matrix_type *matrix2 = matrix_copy_shape(matrix1);

  for (unsigned int row = 0; row < matrix1->rows; row++) {
    for (unsigned int column = 0; column < matrix1->columns; column++) {
      matrix2->matrix[row][column] = matrix1->matrix[row][column];
    }
  } return matrix2;
}

long double sigmoid(long double x) {
  return 1.0 / (1.0 + (long double)expl(-x));
}

matrix_type *matrix_sigmoid(matrix_type *matrix) {
  matrix_for_each(sigmoid, matrix);
  return matrix;
}

matrix_type *matrix_tanh(matrix_type *matrix) {
  matrix_for_each(tanhl, matrix);
  return matrix;
}

long double sigmoid_derivative_helper(long double x) {
  return x * (1.0 - x);
}

long double tanh_derivative_helper(long double x) {
  return 1.0 - x * x;
}

matrix_type *sigmoid_derivative(matrix_type *matrix) {
  matrix_for_each(sigmoid_derivative_helper, matrix);
  return matrix;
}

matrix_type *tanh_derivative(matrix_type *matrix) {
  matrix_for_each(tanh_derivative_helper, matrix);
  return matrix;
}

matrix_type *broadcast_function(long double (*f)(long double, long double), matrix_type *matrix1, matrix_type *matrix2) {
  matrix_type *matrix3 = NULL;
  unsigned int rows1    = matrix1->rows,
               columns1 = matrix1->columns,
               rows2    = matrix2->rows,
               columns2 = matrix2->columns,
               rows3, columns3, A, B, C, D;

  A = rows1    == 1;
  B = columns1 == 1;
  C = rows2    == 1;
  D = columns2 == 1;

  assert(rows1    == rows2    || A || C);
  assert(columns1 == columns2 || B || D);

  rows3    = A ? rows2    : rows1;
  columns3 = B ? columns2 : columns1;
  matrix3  = make_matrix(rows3, columns3);

  for (unsigned int row = 0; row < rows3; row++) {
    for (unsigned int column = 0; column < columns3; column++) {
      matrix3->matrix[row][column] =
        f(matrix1->matrix[A ? 0 : row][B ? 0 : column],
          matrix2->matrix[C ? 0 : row][D ? 0 : column]);
    }
  } destroy_matrix(matrix1);
    destroy_matrix(matrix2);
    return matrix3;
}

long double add(long double x, long double y) {return x + y;}
long double multiply(long double x, long double y) {return x * y;}
long double minus(long double x, long double y) {return x - y;}

matrix_type *fold(unsigned int time, long double (*f)(long double, long double), matrix_type *matrix, ...) {
  va_list args;
  if (time > 1) {
    va_start(args, matrix);
    for (unsigned int n = 1; n < time; n++) {
      matrix = broadcast_function(f, matrix, va_arg(args, matrix_type *));
    } va_end(args);
  }   return matrix;
}

matrix_type *transpose(matrix_type *matrix1) {
  matrix_type *matrix2 = make_matrix(matrix1->columns, matrix1->rows);
  for (unsigned int row = 0; row < matrix1->columns; row++) {
    for (unsigned int column = 0; column < matrix1->rows; column++) {
      matrix2->matrix[row][column] = matrix1->matrix[column][row];
    }
  } destroy_matrix(matrix1);
    return matrix2;
}
