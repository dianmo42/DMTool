#ifndef _VARIABLES_H
#define _VARIABLES_H

extern char *fn_dump;
extern int Nstep;
extern int Natom;
extern double dt;
extern double box[3];
extern double ***dr;
extern double ***r, ***v, **pe, **ke;
extern double rcut;
extern int Nskip, nsample, nrepeat_static, *nrepeat_t_corr, ilevel, *t_corr;
extern int nthread;
extern int nbin;
extern double binsize;
extern double dq, qmax;
extern double a0;
extern double *sigma2;

/* switches of correlation functions    */
extern int irdf;  /* radial distribution function           */
extern int issf;  /* static structure factor                */
extern int imsd;  /* mean-squared displacement              */
extern int ingp;  /* non-Gaussian parameter                 */
extern int isisf; /* self-intermediate scattering function  */
extern int iovlp; /* structural overlap function            */
extern int ifluc; /* energy fluctuation                     */

/* correlation functions                */
extern double *rdf, *cdf;
extern double *ssf;
extern double *msd;
extern double *ngp;
extern double *sisf;
extern double *ovlp;
extern double *Efluc;


#endif