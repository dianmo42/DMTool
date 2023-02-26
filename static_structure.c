#include "functions.h"
#include "variables.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int static_structure(int t0) {
    int i, j;
    int ibin;
    double **E_a;

    if (ifluc) {
        E_a = (double **)malloc(Natom * sizeof(double *));
        for (i = 0; i < Natom; ++i) {
            E_a[i] = (double *)calloc(nbin, sizeof(double));
        }
    }
    distance_matrix(r[t0], r[t0]);

    for (i = 0; i < Natom - 1; ++i) {
        for (j = i + 1; j < Natom; ++j) {
            if (dr[i][j][4] < rcut) {
                ibin = dr[i][j][4] / binsize;

                if (irdf)
                    rdf[ibin] += 2;

                if (ifluc) {
                    E_a[i][ibin] += pe[t0][j];
                    E_a[j][ibin] += pe[t0][i];
                }
            }
        }
    }

    if (ifluc) {
        double Ebar2, E2bar;
        for (i = 0; i < Natom; ++i) {
            E_a[i][0] = pe[t0][i];
            for (ibin = 1; ibin < nbin; ++ibin) {
                E_a[i][ibin] += E_a[i][ibin - 1];
            }
        }

        for (ibin = 0; ibin < nbin; ++ibin) {
            Ebar2 = 0;
            E2bar = 0;
            for (i = 0; i < Natom; ++i) {
                E2bar += pow(E_a[i][ibin], 2);
                Ebar2 += E_a[i][ibin];
            }
            Efluc[ibin] += E2bar / Natom - pow(Ebar2 / Natom, 2);
        }
        for (i = 0; i < nbin; ++i)
            free(E_a[i]);
        free(E_a);
    }

    /* static structure factor */
    if (issf) {
        int iq;
        double q_tmp, cos_tmp, sin_tmp;
        for (iq = 50; iq < nbin; ++iq) {
            cos_tmp = 0;
            sin_tmp = 0;
            q_tmp = iq * pow(dq, 1. / 3.);
#pragma omp parallel for schedule(dynamic) num_threads(nthread) firstprivate(q_tmp) reduction(+:cos_tmp, sin_tmp)
            for (i = 0; i < Natom; ++i) {
                cos_tmp += cos(r[t0][i][0] * q_tmp + r[t0][i][1] * q_tmp + r[t0][i][2] * q_tmp);
                sin_tmp += sin(r[t0][i][0] * q_tmp + r[t0][i][1] * q_tmp + r[t0][i][2] * q_tmp);
            }
            ssf[iq] += cos_tmp * cos_tmp + sin_tmp * sin_tmp;
        }
    }

    return 1;
}