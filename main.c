#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "utils.h"
#include "matrix.h"

int main(int argc, char *argv[])
{
    unsigned int len = (unsigned int)atoi(argv[1]);
    unsigned int threads = (unsigned int)atoi(argv[2]);
    struct timespec start, end;
    matrix_t matA;
    matrix_t matB;
    matrix_t dst;

    clock_gettime(CLOCK_MONOTONIC, &start);
    srand(start.tv_nsec);
    
    assert(new_random_matrix(&matA, len, len) == 0);
    assert(new_random_matrix(&matB, len, len) == 0);
    assert(new_matrix(&dst, len, len) == 0);
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    assert(matmul_p(matA, matB, dst, threads) == 0);
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
