#include "variables.h"
#include <math.h>
#include <stdio.h>

int output(int iout) {
    int i, ibin;
    switch (iout) {
    case 1:

        if (irdf) {
            double vshell;
            double rho = Natom / (box[0] * box[1] * box[2]);

            /* cumulative distribution function */
            cdf[0] = nrepeat_static * Natom;
            for (ibin = 1; ibin < nbin; ++ibin) {
                cdf[ibin] = cdf[ibin - 1] + rdf[ibin - 1];
            }

            for (ibin = 0; ibin < nbin; ++ibin) {
                vshell = 4 * M_PI * (pow(ibin + 1, 3) - pow(ibin, 3)) *
                         pow(binsize, 3) / 3.;
                rdf[ibin] /= vshell * rho;
            }

            FILE *fp = fopen("rdf.dat", "w");

            for (ibin = 0; ibin < nbin; ++ibin) {
                rdf[ibin] /= nrepeat_static * Natom;
                cdf[ibin] /= nrepeat_static * Natom;
                fprintf(fp, "%lf %lf %lf\n", ibin * binsize, rdf[ibin],
                        cdf[ibin]);
            }
            fclose(fp);
        }

        if (issf) {
            int iq;
            FILE *fp = fopen("ssf.dat","w");
            for (iq = 50; iq < nbin; ++iq) {
                ssf[iq] /= nrepeat_static * Natom;
                fprintf(fp, "%lf %lf\n", iq * dq, ssf[iq]);
            }
            fclose(fp);
        }

        if (ifluc) {
            double E2bar, Ebar2;
            FILE *fp = fopen("fluc.dat", "w");
            for (ibin = 0; ibin < nbin; ++ibin) {
                Efluc[ibin] /= nrepeat_static * pow(cdf[ibin], 2);
                fprintf(fp, "%lf %lf\n", ibin * binsize, Efluc[ibin]);
            }
        }
        break;

    case 2:
        if (imsd) {
            FILE *fp = fopen("msd.dat", "w");
            for (i = 0; i < nsample; ++i) {
                msd[i] /= nrepeat_t_corr[i];
                ngp[i] /= nrepeat_t_corr[i];
                fprintf(fp, "%lf %lf %lf\n", t_corr[i] * dt, msd[i], ngp[i]);
            }
            fclose(fp);
        }

        if (isisf) {
            FILE *fp = fopen("sisf.dat", "w");
            for (i = 0; i < nsample; ++i) {
                sisf[i] /= Natom * nrepeat_t_corr[i];
                fprintf(fp,"%.3f %lf\n", t_corr[i] * dt, sisf[i]);
            }
            fclose(fp);
        }

        if (iovlp) {
            FILE *fp = fopen("ovlp.dat", "w");
            for (i = 0; i < nsample; ++i) {
                ovlp[i] /= Natom * nrepeat_t_corr[i];
                fprintf(fp,"%.3f %lf\n", t_corr[i] * dt, ovlp[i]);
            }
            fclose(fp);
        }
        break;

    default:
        break;
    }

    return 1;
}