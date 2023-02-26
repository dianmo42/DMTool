#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

/* read parameters  */
int read_parameter();

/* allocate memory  */
int alloc_mem();

/* read dump file   */
int read_dump();

/* distance matrix  */
int distance_matrix(double **r_1, double **r_2);

/* static functions */
int static_structure(int t0);

/* time correlation function */
int time_correlation(int t0, int t1, int isample);

/* print results    */
int output(int type);

#endif