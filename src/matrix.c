#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "matrix.h"

void matmul(matrix_t matA, matrix_t matB, matrix_t dst)
{
    int result = 0;
    unsigned int len = 0;

    if (MATMUL_AVAILABILITY_CHECK(matA, matB, dst) == false)
    {
        fprintf(stderr, "%s: invalid shape\n", __func__);
        exit(EXIT_FAILURE);
    }
    len = dst.len;
    for (unsigned int i = 0; i < len; i++)
    {
        for (unsigned int j = 0; j < len; j++)
        {
            result = 0;
            for (unsigned int k = 0; k < len; k++)
            {
                result += matA.data[i + len * k] * matB.data[k + len * j];
            }
            dst.data[i + len * j] = result;
        }
    }
}

void matmul_p(matrix_t matA, matrix_t matB, matrix_t dst)
{
    int ret = 0;
    unsigned int len = 0;
    pthread_t *a_thread;
    
    if (MATMUL_AVAILABILITY_CHECK(matA, matB, dst) == false)
    {
        fprintf(stderr, "%s: invalid shape\n", __func__);
        exit(EXIT_FAILURE);
    }
    len = dst.len;

    a_thread = (pthread_t *)malloc(sizeof(pthread_t) * len);
    if (a_thread == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    for (unsigned int tnum = 0; tnum < len; tnum++)
    {
        matmul_p_routine_args_t *args;
        args = (matmul_p_routine_args_t *)malloc(sizeof(matmul_p_routine_args_t));
        if (args == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        args->matA = matA;
        args->matB = matB;
        args->dst = dst;
        args->num_col = tnum;

        ret = pthread_create(a_thread + tnum, NULL, matmul_p_routine, (void *)args);
        if (ret != 0)
        {
            fprintf(stderr, "pthread_create: thread create failed (code %d)\n", ret);
            exit(EXIT_FAILURE);
        }
        else
        {
#ifdef DEBUG_LOG
            printf("thread %d created.\n", tnum);
#endif
        }
    }

    for (unsigned int tnum = 0; tnum < len; tnum++)
    {
        ret = pthread_join(a_thread[tnum], (void **)NULL);
        if (ret != 0)
        {
            fprintf(stderr, "pthread_join: thread join failed (code %d)\n", ret);
            exit(EXIT_FAILURE);
        }
        else
        {
#ifdef DEBUG_LOG
            printf("thread %d joined.\n", tnum);
#endif
        }
    }
    
    free(a_thread);
}

void *matmul_p_routine(void *args)
{
    int result = 0;
    matrix_t matA = ((matmul_p_routine_args_t *)args)->matA;
    matrix_t matB = ((matmul_p_routine_args_t *)args)->matB;
    matrix_t dst = ((matmul_p_routine_args_t *)args)->dst;
    unsigned int len = ((matmul_p_routine_args_t *)args)->matA.len;
    unsigned int num_col = ((matmul_p_routine_args_t *)args)->num_col;
    free(args);

    for (unsigned int j = 0; j < len; j++)
    {
        result = 0;
        for (unsigned int k = 0; k < len; k++)
        {
            result += matA.data[num_col + len * k] * matB.data[k + len * j];
        }
        dst.data[num_col + len * j] = result;
    }

    return NULL;
}