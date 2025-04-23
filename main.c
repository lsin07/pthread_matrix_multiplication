#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "matrix.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s matrix_size max_threads\n", argv[0]);
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
    fprintf(fp, "tn,time\n");

    // Loop through different numbers of threads from 1 to num_threads_max
    unsigned int rep = 32;

    // Generate a list of thread counts in random order
    unsigned int* thread_counts = malloc(max_threads * sizeof(unsigned int));
    for (unsigned int i = 0; i < max_threads; ++i) {
        thread_counts[i] = i + 1;
    }
    // Shuffle the array using Fisher-Yates algorithm
    for (unsigned int i = max_threads - 1; i > 0; --i) {
        unsigned int j = rand() % (i + 1);
        unsigned int temp = thread_counts[i];
        thread_counts[i] = thread_counts[j];
        thread_counts[j] = temp;
    }

    for (unsigned int i = 0; i < max_threads; ++i) {
        unsigned int current_tn = thread_counts[i];
        double total_time = 0.0;
        printf("(%u / %u) processing tn = %u ", i + 1, max_threads, current_tn);
        fflush(stdout);
        for (unsigned int run = 0; run < rep; run++) {
            clock_gettime(CLOCK_MONOTONIC, &start);
            matmul(matA, matB, dst, current_tn);
            clock_gettime(CLOCK_MONOTONIC, &end);
            double this_time = timediff(start, end);
            total_time += this_time;

            if ((rep >= 10) && (run % (rep / 10) == rep / 10 - 1)) {
                putchar('.');
                fflush(stdout);
            }
            fprintf(fp, "%u,%.9lf\n", current_tn, this_time);
        }

        // Calculate average time
        double avg_time = total_time / (double)rep;
        printf(" avg_time = %.9lf\n", avg_time);
    }

    fclose(fp);
    free(thread_counts);

    // print_matrix(matA, "A");
    // print_matrix(matB, "B");
    // print_matrix(dst, "result");

    del_matrix(&matA);
    del_matrix(&matB);
    del_matrix(&dst);

    printf("finished.\n");

    return 0;
}
