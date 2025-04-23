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
    unsigned int start;
    unsigned int num_tasks;
} matmul_p_routine_args_t;

void matmul(const matrix_t matA, const matrix_t matB, const matrix_t dst, const unsigned int num_threads);

#endif