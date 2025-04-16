#ifndef _MATRIX_H
#define _MATRIX_H

#define MATMUL_AVAILABILITY_CHECK(matA, matB, dst) ((matA.len == matB.len) && (matB.len == dst.len))

typedef struct _matrix_t
{
    int* data;
    unsigned int len;
} matrix_t;

typedef struct _matmul_p_routine_args_t
{
    matrix_t matA;
    matrix_t matB;
    matrix_t dst;
    unsigned int num_col;
} matmul_p_routine_args_t;

void matmul(matrix_t matA, matrix_t matB, matrix_t dst);
void matmul_p(matrix_t matA, matrix_t matB, matrix_t dst);
void *matmul_p_routine(void *args);

#endif