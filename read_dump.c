#include "functions.h"
#include "variables.h"
#include <stdio.h>
#include <stdlib.h>

int read_dump(int stage) {
    int i;
    int istep, iatom;
    double xlo, xhi, ylo, yhi, zlo, zhi;
    double rx_tmp, ry_tmp, rz_tmp, vx_tmp, vy_tmp, vz_tmp, ke_tmp, pe_tmp;
    static FILE *fp_dump;

    switch (stage) {
    case 1:

        if ((fp_dump = fopen(fn_dump, "r")) == 0) {
            fprintf(stderr, "Error: can not open file %s\n", fn_dump);
            exit(1);
        }

        /* system information */
        for (i = 0; i < 3; ++i)
            fscanf(fp_dump, "%*[^\n]\n");
        fscanf(fp_dump, "%d\n", &Natom);
        fscanf(fp_dump, "%*[^\n]\n");
        fscanf(fp_dump, "%lf %lf\n", &xlo, &xhi);
        fscanf(fp_dump, "%lf %lf\n", &ylo, &yhi);
        fscanf(fp_dump, "%lf %lf\n", &zlo, &zhi);
        box[0] = xhi - xlo;
        box[1] = yhi - ylo;
        box[2] = zhi - zlo;
        for (i = 0; i < Natom + 1; ++i)
            fscanf(fp_dump, "%*[^\n]\n");
        break;

    case 2:
        for (istep = 0; istep < Nstep; ++istep) {
            for (i = 0; i < 9; ++i)
                fscanf(fp_dump, "%*[^\n]\n");
            for (i = 0; i < Natom; ++i) {
                fscanf(fp_dump, "%d %lf %lf %lf %lf %lf\n", &iatom, 
                        &rx_tmp, &ry_tmp, &rz_tmp, 
                        // &vx_tmp, &vy_tmp, &vz_tmp, 
                        &ke_tmp, &pe_tmp);
                iatom -= 1;
                r[istep][iatom][0] = rx_tmp;
                r[istep][iatom][1] = ry_tmp;
                r[istep][iatom][2] = rz_tmp;
                // v[istep][iatom][0] = vx_tmp;
                // v[istep][iatom][1] = vy_tmp;
                // v[istep][iatom][2] = vz_tmp;
                ke[istep][iatom] = ke_tmp;
                pe[istep][iatom] = pe_tmp;
            }
        }
        break;

    default:
        break;
    }

    return 1;
}
