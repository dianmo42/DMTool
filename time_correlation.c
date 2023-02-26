#include "functions.h"
#include "variables.h"
#include <math.h>
#include <stdio.h>

int time_correlation(int t0, int t1, int isample) {
    int i, j;
    distance_matrix(r[t0], r[t1]);

    if (imsd) {
        double msd_tmp = 0, ngp_tmp = 0;
#pragma omp parallel for schedule(dynamic) num_threads(nthread) firstprivate(dr) reduction(+ : msd_tmp)
        for (i = 0; i < Natom; ++i) {
            msd_tmp += dr[i][i][3];
        }
        msd_tmp /= Natom;

#pragma omp parallel for schedule(dynamic) num_threads(nthread) firstprivate(dr) reduction(+ : ngp_tmp)
        for (i = 0; i < Natom; ++i) {
            ngp_tmp += pow(dr[i][i][3], 2);
        }
        ngp_tmp = 0.6 * ngp_tmp / Natom / (msd_tmp * msd_tmp) - 1;
        msd[isample] += msd_tmp;
        ngp[isample] += ngp_tmp;
    }

    if (isisf) {
        double sisf_tmp = 0;
#pragma omp parallel for schedule(dynamic) num_threads(nthread) reduction(+ : sisf_tmp)
        for (i = 0; i < Natom; ++i) {
            sisf_tmp += cos(dr[i][i][4] * qmax);
        }
        if (sisf_tmp > 0)
            sisf[isample] += sisf_tmp;
    }

    if (iovlp) {
        double ovlp_tmp = 0;
#pragma omp parallel for schedule(dynamic) num_threads(nthread) firstprivate(a0) reduction(+ : ovlp_tmp)
        for (i = 0; i < Natom; ++i) {
            if (dr[i][i][4] < a0)
                ++ovlp_tmp;
        }
        ovlp[isample] += ovlp_tmp;
    }

    ++nrepeat_t_corr[isample];
    return 1;
}