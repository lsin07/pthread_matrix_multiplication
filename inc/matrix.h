#ifndef _MATRIX_H
#define _MATRIX_H

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

void matmul_np(matrix_t matA, matrix_t matB, matrix_t dst);
void matmul_p(matrix_t matA, matrix_t matB, matrix_t dst);

#endif
