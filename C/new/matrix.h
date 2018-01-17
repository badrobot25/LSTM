// matrix.h
#ifndef MATRIX_H
  #define MATRIX_H
  #include <stdlib.h>
  #include <assert.h>
  #include <math.h>
  #include <stdarg.h>

  #define NOT_USED(x) ((void)x)

  typedef struct {
    unsigned int rows, columns;
    long double **matrix;
  } matrix;

  matrix *make_matrix(unsigned int, unsigned int);
  matrix *destroy_matrix(matrix *);
  long double random_long_double(long double);
  long double zero(long double);
  long double one(long double);
  void matrix_for_each(long double (*)(long double), matrix *);
  matrix *dot_product(matrix *, matrix *);
  matrix *matrix_copy_shape(matrix *);
  matrix *matrix_copy(matrix *);
  matrix *matrix_initialize(long double (*)(long double), unsigned int, unsigned int);
  matrix *matrix_initialize_from_matrix(long double (*)(long double), matrix *);
  long double sigmoid(long double);
  matrix *matrix_sigmoid(matrix *);
  matrix *matrix_tanh(matrix *);
  long double sigmoid_derivative_helper(long double);
  long double tanh_derivative_helper(long double);
  matrix *sigmoid_derivative(matrix *);
  matrix *tanh_derivative(matrix *);
  matrix *matrix_map2(long double (*)(long double, long double), matrix *, matrix *);
  matrix *fold(long double (*)(long double, long double), long double (*)(long double), unsigned int, matrix *, va_list);
  long double add(long double, long double);
  long double multiply(long double, long double);
  long double minus(long double, long double);
  matrix *sum(unsigned int, matrix *, ...);
  matrix *product(unsigned int, matrix *, ...);
  matrix *subtract(unsigned int, matrix *, ...);
  matrix *transpose(matrix *);

#endif
