#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "matrix.h"
#include "utils.h"

static void __transpose(const matrix_t mat, matrix_t* const dst);
static void __matmul_np(const matrix_t matA, const matrix_t matB, const matrix_t dst);
static void __matmul_p(const matrix_t matA, const matrix_t matB, const matrix_t dst, const unsigned int num_threads);
static void *__matmul_p_routine(void *args);

static void __transpose(const matrix_t mat, matrix_t* const dst)
{
    int* new_data;
    unsigned int len = mat.len;
    new_data = (int *)calloc(len * len, sizeof(int));
    if (new_data == NULL)
    {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    for (unsigned int i = 0; i < len; i++)
    {
        for (unsigned int j = 0; j < len; j++)
        {
            new_data[i * len + j] = mat.data[j * len + i];
        }
    }

    dst->data = new_data;
    dst->len = len;
}

static void __matmul_np(const matrix_t matA, const matrix_t matB, const matrix_t dst)
{
    int result = 0;
    unsigned int len = dst.len;
    matrix_t matB_T;

    __transpose(matB, &matB_T);
    for (unsigned int i = 0; i < len; i++)
    {
        for (unsigned int j = 0; j < len; j++)
        {
            result = 0;
            for (unsigned int k = 0; k < len; k++)
            {
                result += matA.data[i * len + k] * matB_T.data[j * len + k];
            }
            dst.data[i * len + j] = result;
        }
    }
    
    del_matrix(&matB_T);
}

static void __matmul_p(const matrix_t matA, const matrix_t matB, const matrix_t dst, unsigned int num_threads)
{
    int ret = 0;
    unsigned int len = dst.len;
    pthread_t *threads_arr;
    matrix_t matB_T;

    threads_arr = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
    if (threads_arr == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    __transpose(matB, &matB_T);

    unsigned int len_sq = len * len;
    unsigned int num_th = len_sq > num_threads ? num_threads : len_sq;
    unsigned int tasks_per_thread = len_sq / num_th;
    unsigned int rem = len_sq % num_th;
    unsigned int start = 0;
    for (unsigned int tnum = 0; tnum < num_th; tnum++)
    {
        matmul_p_routine_args_t *args;
        args = (matmul_p_routine_args_t *)malloc(sizeof(matmul_p_routine_args_t));
        if (args == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        args->matA = matA;
        args->matB = matB_T;
        args->dst = dst;
        args->start = start;
        args->num_tasks = tnum < rem ? tasks_per_thread + 1 : tasks_per_thread;
        start += args->num_tasks;
        
        ret = pthread_create(threads_arr + tnum, NULL, __matmul_p_routine, (void *)args);
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
    
    for (unsigned int tnum = 0; tnum < num_th; tnum++)
    {
        ret = pthread_join(threads_arr[tnum], NULL);
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
    
    free(threads_arr);
    del_matrix(&matB_T);
}

static void *__matmul_p_routine(void *args)
{
    matrix_t matA = ((matmul_p_routine_args_t *)args)->matA;
    matrix_t matB = ((matmul_p_routine_args_t *)args)->matB;
    matrix_t dst = ((matmul_p_routine_args_t *)args)->dst;
    unsigned int len = ((matmul_p_routine_args_t *)args)->matA.len;
    unsigned int start = ((matmul_p_routine_args_t *)args)->start;
    unsigned int num_tasks = ((matmul_p_routine_args_t *)args)->num_tasks;
    free(args);

    int row, col;
    for (unsigned int task = start; task < start + num_tasks; task++)
    {
        row = task / len;
        col = task % len;
        dst.data[row * len + col] = 0;
        for (unsigned int i = 0; i < len; i++)
        {
            dst.data[row * len + col] += matA.data[row * len + i] * matB.data[col * len + i];
        }
    }

    return NULL;
}

void matmul(const matrix_t matA, const matrix_t matB, const matrix_t dst, const unsigned int num_threads)
{
    if ((matA.len != matB.len) || (matB.len != dst.len))
    {
        fprintf(stderr, "%s: invalid shape\n", __func__);
        exit(EXIT_FAILURE);
    }
    else
    {
        if (num_threads < 1)
        {
            fprintf(stderr, "%s: thread_num must be equal or bigger than 1\n", __func__);
            exit(EXIT_FAILURE);
        }
        else if (num_threads == 1)
        {
            __matmul_np(matA, matB, dst);
        }
        else
        {
            __matmul_p(matA, matB, dst, num_threads);
        }
    }

}
