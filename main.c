#include "functions.h"
#include "variables.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

char *fn_dump;
int Nstep, Natom;
double dt;
double box[3];
double ***dr;
double ***r, ***v, **pe, **ke;
double rcut;
int Nskip, nsample, nrepeat_static, *nrepeat_t_corr, ilevel, *t_corr;
int nthread;
int nbin;
double binsize;
double dq, qmax;
double a0;

/* switches of correlation functions    */
int irdf;  /* radial distribution function           */
int issf;  /* static structure factor                */
int imsd;  /* mean-squared displacement              */
int ingp;  /* non-Gaussian parameter                 */
int isisf; /* self-intermediate scattering function  */
int iovlp; /* structural overlap function            */
int ifluc; /* energy fluctuation                     */

/* correlation functions                */
double *rdf, *cdf;
double *ssf;
double *msd;
double *ngp;
double *sisf;
double *ovlp;
double *Efluc;

int main(int argc, char **argv) {

    int i, j, t0, t1;

    /* read parameters  */
    read_parameter();

    /* read dump file   */
    read_dump(1);

    /* allocate memory  */
    alloc_mem();

    /* read dump file   */
    read_dump(2);

    /* static properties */
    for (t0 = 0; t0 < Nstep; t0 += Nskip) {
        static_structure(t0);
    }
    output(1);

    /* time correlation functions */
    for (t0 = 0; t0 < Nstep; t0 += Nskip) {
        for (i = 0; i < nsample; ++i) {
            t1 = t0 + t_corr[i];
            if (t1 < Nstep) {
                time_correlation(t0, t1, i);
            }
        }
    }
    output(2);

    return 0;
}