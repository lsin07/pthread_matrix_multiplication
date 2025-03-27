#ifndef _UTILS_H
#define _UTILS_H

#include "matrix.h"

int new_matrix(matrix_t *mat, const unsigned int len);
int new_random_matrix(matrix_t *mat, const unsigned int len);
void print_matrix(const matrix_t mat, const char* name);
void del_matrix(matrix_t *mat);
double timediff(const struct timespec start, const struct timespec end);

#endif