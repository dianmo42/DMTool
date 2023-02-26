#include "functions.h"
#include "variables.h"
#include <math.h>
#include <omp.h>

/* The distance matrix of two configuration.
 * dx, dy, and dz between the i-th and j-th atoms are
 * stored in dr[i][j][0], dr[i][j][1], dr[i][j][2].
 * dr[i][j][3] is dr2 = dx * dx + dy * dy + dz * dz
 * dr[i][j][4] is dr = sqrt(dr2). */

int distance_matrix(double **r_1, double **r_2) {
    int i, j;

/* distance matrix */
#pragma omp parallel for schedule(dynamic) num_threads(nthread) private(j)     \
    firstprivate(box)
    for (i = 0; i < Natom; ++i) {

        for (j = i; j < Natom; ++j) {
            dr[i][j][0] = r_1[j][0] - r_2[i][0];
            dr[i][j][1] = r_1[j][1] - r_2[i][1];
            dr[i][j][2] = r_1[j][2] - r_2[i][2];

            /* wrap priodic boundary condition */
            if (dr[i][j][0] < -0.5) {
                ++dr[i][j][0];
            } else if (dr[i][j][0] > 0.5) {
                --dr[i][j][0];
            }
            if (dr[i][j][1] < -0.5) {
                ++dr[i][j][1];
            } else if (dr[i][j][1] > 0.5) {
                --dr[i][j][1];
            }
            if (dr[i][j][2] < -0.5) {
                ++dr[i][j][2];
            } else if (dr[i][j][2] > 0.5) {
                --dr[i][j][2];
            }

            dr[i][j][0] *= box[0];
            dr[i][j][1] *= box[1];
            dr[i][j][2] *= box[2];
            dr[i][j][3] =
                pow(dr[i][j][0], 2) + pow(dr[i][j][1], 2) + pow(dr[i][j][2], 2);
            dr[i][j][4] = sqrt(dr[i][j][3]);

            dr[j][i][0] = -dr[i][j][0];
            dr[j][i][1] = -dr[i][j][1];
            dr[j][i][2] = -dr[i][j][2];
            dr[j][i][3] = dr[i][j][3];
            dr[j][i][4] = dr[i][j][4];
        }
    }

    return 1;
}