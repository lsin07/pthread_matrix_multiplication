#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "matrix.h"

#define MATMUL_AVAILABILITY_CHECK(matA, matB, dst) ((matA.row == matB.col) && (matA.col == dst.col) && (matB.row == dst.row))

int matmul(matrix_t matA, matrix_t matB, matrix_t dst)
{
    int result = 0;

    if (MATMUL_AVAILABILITY_CHECK(matA, matB, dst) == false)
    {
        return -1;
    }

    for (int i = 0; i < matA.col; i++)
    {
        for (int j = 0; j < matB.row; j++)
        {
            result = 0;
            for (int k = 0; k < matA.row; k++)
            {
                result += matA.data[i + matA.col * k] * matB.data[k + matB.col * j];
            }
            dst.data[i + dst.col * j] = result;
        }
    }
    
    return 0;
}

int matmul_p(matrix_t matA, matrix_t matB, matrix_t dst, unsigned int num_t)
{
    int ret = 0;
    unsigned int start = 0;
    unsigned int num_threads, col_per_thread, rem;
    pthread_t *a_thread;
    
    if (MATMUL_AVAILABILITY_CHECK(matA, matB, dst) == false)
    {
        return -1;
    }

    num_threads = num_t > matA.col ? matA.col : num_t;
    col_per_thread = matA.col / num_t;
    rem = matA.col % num_t;

    a_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
    if (a_thread == NULL)
    {
        perror("");
        return -1;
    }
    
    for (int tnum = 0; tnum < num_threads; tnum++)
    {
        matmul_p_routine_args_t *args;
        args = (matmul_p_routine_args_t *)malloc(sizeof(matmul_p_routine_args_t));
        if (args == NULL)
        {
            perror("");
            return -1;
        }
        args->matA = matA;
        args->matB = matB;
        args->dst = dst;
        args->col_start = start;
        args->num_col = tnum < rem ? col_per_thread + 1 : col_per_thread;
        start += args->num_col;

        ret = pthread_create(a_thread + tnum, NULL, matmul_p_routine, (void *)args);
        if (ret != 0)
        {
            fprintf(stderr, "pthread_create: thread create failed (code %d)\n", ret);
            return -1;
        }
        else
        {
#ifdef DEBUG_LOG
            printf("thread %d created.\n", tnum);
#endif
        }
    }

    for (int tnum = 0; tnum < num_threads; tnum++)
    {
        ret = pthread_join(a_thread[tnum], (void **)NULL);
        if (ret != 0)
        {
            fprintf(stderr, "pthread_join: thread join failed (code %d)\n", ret);
            return -1;
        }
        else
        {
#ifdef DEBUG_LOG
            printf("thread %d joined.\n", tnum);
#endif
        }
    }
    
    free(a_thread);
    return 0;
}

void *matmul_p_routine(void *args)
{
    int result = 0;
    matrix_t matA = ((matmul_p_routine_args_t *)args)->matA;
    matrix_t matB = ((matmul_p_routine_args_t *)args)->matB;
    matrix_t dst = ((matmul_p_routine_args_t *)args)->dst;
    unsigned int col_start = ((matmul_p_routine_args_t *)args)->col_start;
    unsigned int num_col = ((matmul_p_routine_args_t *)args)->num_col;
    free(args);

    for (int i = col_start; i < col_start + num_col; i++)
    {
        for (int j = 0; j < matB.row; j++)
        {
            result = 0;
            for (int k = 0; k < matA.row; k++)
            {
                result += matA.data[i + matA.col * k] * matB.data[k + matB.col * j];
            }
            dst.data[i + dst.col * j] = result;
        }
    }

    return (void **)NULL;
}