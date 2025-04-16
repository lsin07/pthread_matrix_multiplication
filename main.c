#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "utils.h"
#include "matrix.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s matrix_size num_threads\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    unsigned int len = (unsigned int)atoi(argv[1]);
    unsigned int num_threads = (unsigned int)atoi(argv[2]);
    struct timespec start, end;
    matrix_t matA;
    matrix_t matB;
    matrix_t dst;

    clock_gettime(CLOCK_MONOTONIC, &start);
    srand((unsigned int)(start.tv_nsec));
    
    new_random_matrix(&matA, len);
    new_random_matrix(&matB, len);
    new_matrix(&dst, len);
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    matmul_p(matA, matB, dst, num_threads);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("took %.9lfs\n", timediff(start, end));

    // print_matrix(matA, "A");
    // print_matrix(matB, "B");
    // print_matrix(dst, "result");

    del_matrix(&matA);
    del_matrix(&matB);
    del_matrix(&dst);
    return 0;
}
