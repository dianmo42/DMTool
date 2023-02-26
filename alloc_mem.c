#include "variables.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int alloc_mem() {
    int i, j;
    pe = (double **)malloc(Nstep * sizeof(double *));
    ke = (double **)malloc(Nstep * sizeof(double *));
    r = (double ***)malloc(Nstep * sizeof(double **));
    v = (double ***)malloc(Nstep * sizeof(double **));

    for (int istep = 0; istep < Nstep; ++istep) {
        pe[istep] = (double *)malloc(Natom * sizeof(double));
        ke[istep] = (double *)malloc(Natom * sizeof(double));
        r[istep] = (double **)malloc(Natom * sizeof(double *));
        v[istep] = (double **)malloc(Natom * sizeof(double *));

        for (i = 0; i < Natom; ++i) {
            r[istep][i] = (double *)malloc(3 * sizeof(double));
            v[istep][i] = (double *)malloc(3 * sizeof(double));
        }
    }

    if (ilevel == 0 || ilevel == 1) {
        nsample = Nstep / 2 - 1;
        t_corr = (int *)malloc(nsample * sizeof(int));
        for (i = 0; i < nsample; ++i)
            t_corr[i] = i;
    } else if (ilevel == -1) {
        nsample = (int)(log10(Nstep)) * 9 +
                  (int)(Nstep / pow(10, floor(log10(Nstep)))) - 1;
        t_corr = (int *)malloc(nsample * sizeof(int));
        for (i = 0; i < nsample; ++i) {
            t_corr[i] = (int)pow(10, i / 9) * (i + 1 - 9 * (i / 9));
        }
    } else {
        printf("Error: ilevel must be -1, 0, 1, 2, 3...\n");
        exit(1);
    }

    nrepeat_static = Nstep / Nskip;
    nrepeat_t_corr = (int *)calloc(nsample, sizeof(int));
    binsize = rcut / nbin;
    dr = (double ***)malloc(Natom * sizeof(double **));
    for (i = 0; i < Natom; ++i) {
        dr[i] = (double **)malloc(Natom * sizeof(double *));
        for (j = 0; j < Natom; ++j) {
            dr[i][j] = (double *)malloc(4 * sizeof(double));
        }
    }

    if (ifluc) {
        irdf = 1;
        Efluc = (double *)calloc(nbin, sizeof(double *));
    }

    if (irdf) {
        rdf = (double *)calloc(nbin, sizeof(double));
        cdf = (double *)calloc(nbin, sizeof(double));
    }

    if (issf) {
        ssf = (double *)calloc(nbin, sizeof(double));
        dq = 0.02;
    }

    if (imsd) {
        msd = (double *)calloc(nsample, sizeof(double));
        ngp = (double *)calloc(nsample, sizeof(double));
    }

    if (isisf) {
        sisf = (double *)calloc(nsample, sizeof(double));
    }

    if (iovlp)
        ovlp = (double *)calloc(nsample, sizeof(double));

    return 1;
}
