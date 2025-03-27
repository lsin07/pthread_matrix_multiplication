#ifndef _MATRIX_H
#define _MATRIX_H

typedef struct _matrix_t
{
    int* data;
    unsigned int row;
    unsigned int col;
} matrix_t;

typedef struct _matmul_p_routine_args_t
{
    matrix_t matA;
    matrix_t matB;
    matrix_t dst;
    unsigned int col_start;
    unsigned int num_col;
} matmul_p_routine_args_t;

int matmul(matrix_t matA, matrix_t matB, matrix_t dst);
int matmul_p(matrix_t matA, matrix_t matB, matrix_t dst, unsigned int num_t);
void *matmul_p_routine(void *args);

#endif