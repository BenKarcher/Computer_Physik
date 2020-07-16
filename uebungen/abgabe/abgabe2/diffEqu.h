#ifndef DERHS_H_INCLUDED
#define DERHS_H_INCLUDED

#include "h2.h"
#include <complex.h>

void euler_step(int nequ, int nargs, double h, double t, double y[], double dy_dt[], double args[],
           void (*diffEqu)(int nequ, int nargs, double t, double y[], double dy_dt[], double args[]));

void RuKu4_step(int nequ, int nargs, double h, double t, double y[], double yh[], double *k[4], double args[],
           void (*diffEqu)(int nequ, int nargs, double t, double y[], double dy_dt[], double args[]));

void diff_harmonic(int nequ, int nargs, double t, double y[], double dy_dt[], double w_g[]);
double complex exact_solution(double t, double v0, double x0, double w, double g);

void diff_external_harmonic(int nequ, int nargs, double t, double y[], double dy_dt[], double w_g_wk_a[]);
double complex exact_external_solution(double t, double v0, double x0, double w, double g, double wk, double a);

void diff_unkown_harmonic(int nequ, int nargs, double t, double y[], double dy_dt[], double w_g[]);

FILE *safeOpen(const char *name);

#endif //DERHS_H_INCLUDED
