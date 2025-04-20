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
    unsigned int max_threads = (unsigned int)atoi(argv[2]);
    struct timespec start, end;
    matrix_t matA;
    matrix_t matB;
    matrix_t dst;

    clock_gettime(CLOCK_MONOTONIC, &start);
    srand((unsigned int)(start.tv_nsec));
    
    new_random_matrix(&matA, len);
    new_random_matrix(&matB, len);
    new_matrix(&dst, len);
    
    // Create a CSV file to store the results
    FILE *fp = fopen("result.csv", "w");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Header for the CSV
    fprintf(fp, "num_threads,average_time\n");

    // Loop through different numbers of threads from 1 to num_threads_max
    unsigned int rep = 100;
    for (unsigned int nt = 1; nt <= max_threads; nt++) {
        double total_time = 0.0;
        
        // Run matmul 10 times and measure the average time
        printf("processing tn = %u ... ", nt);
        fflush(stdout);
        for (unsigned int run = 0; run < rep; run++) {
            clock_gettime(CLOCK_MONOTONIC, &start);
            matmul(matA, matB, dst, nt);
            clock_gettime(CLOCK_MONOTONIC, &end);
            total_time += timediff(start, end);
        }

        // Calculate average time
        double avg_time = total_time / (double)rep;
        printf("avg_time = %.9lf\n", avg_time);

        // Write to CSV
        fprintf(fp, "%u,%.9lf\n", nt, avg_time);
    }

    // Close the file
    fclose(fp);

    // print_matrix(matA, "A");
    // print_matrix(matB, "B");
    // print_matrix(dst, "result");

    del_matrix(&matA);
    del_matrix(&matB);
    del_matrix(&dst);

    printf("finished.\n");

    return 0;
}
