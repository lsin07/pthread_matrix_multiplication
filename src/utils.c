#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

int new_matrix(matrix_t *mat, const unsigned int row, const unsigned int col)
{
    int* data;
    data = (int *)calloc(row * col, sizeof(int));
    if (data == NULL)
    {
        perror("");
        return -1;
    }

    mat->data = data;
    mat->row = row;
    mat->col = col;
    return 0;
}

int new_random_matrix(matrix_t *mat, const unsigned int row, const unsigned int col)
{
    int ret;
    ret = new_matrix(mat, row, col);
    if (ret != 0)
    {
        return -1;
    }
    for (int i = 0; i < row * col; i++)
    {
        mat->data[i] = rand() % 10;
    }

    return 0;
}

void print_matrix(const matrix_t mat, const char* name)
{
    printf("===== Matrix %s =====\n", name);
    for (int i = 0; i < mat.row; i++)
    {
        for (int j = 0; j < mat.col; j++)
        {
            printf("%5d", mat.data[j + mat.col * i]);
        }
        printf("\n");
    }
}

void del_matrix(matrix_t *mat)
{
    free(mat->data);
    mat->data = NULL;
    mat->col = 0;
    mat->row = 0;
}

double timediff(const struct timespec start, const struct timespec end)
{
    time_t sec = end.tv_sec - start.tv_sec;
    long ns = end.tv_nsec - start.tv_nsec;
    return (double)sec + (double)ns * 1e-9;
}