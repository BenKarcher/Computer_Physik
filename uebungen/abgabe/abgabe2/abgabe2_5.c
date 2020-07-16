//gcc -Wall abgabe2_5.c diffEqu.c -o abgabe.out -lm
//./abgabe.out
//Ben Karcher, Annika Hoverath
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "diffEqu.h"

int main(void)
{
    double t0=0;
    double tend=100;
    double h=0.001;

    double y0 = 1.5;
    double v0 = 2.75;
    double w = 1;
    double g = 0.1;
    double wk=1.5;
    double a=0.5;

    double y[2] = {y0,v0};
    double dy_dt[2];
    double args[4] = {w,g,wk,a};
    FILE *fout=safeOpen("euler_extern");
    fprintf(fout, "t x_{euler} x_{exact} Differenz\n");
    for (double t = t0; t <= tend; t += h)
    {
        double tmp = creal(exact_external_solution(t, v0, y0,w,g,wk,a));
        fprintf(fout, "%20.5le %20.5le %20.5le %20.5le\n", t, y[0], tmp, y[0] - tmp);

        euler_step(2, 4, h, t, y, dy_dt, args,diff_external_harmonic);
    }

    double k[4][2];
    double *kp[4]={k[0],k[1],k[2],k[3]};
    y[0]=y0;
    y[1]=v0;
    fout=safeOpen("RuKu4_extern");
    fprintf(fout, "t x_{RuKu4} x_{exact} Differenz\n");
    for (double t = t0; t <= tend; t += h)
    {
        double tmp = creal(exact_external_solution(t, v0, y0,w,g,wk,a));
        fprintf(fout, "%20.5le %20.5le %20.5le %20.5le\n", t, y[0], tmp, y[0] - tmp);

        RuKu4_step(2, 4, h, t, y, dy_dt,kp, args,diff_external_harmonic); /* Dgl.schritt ausfuehren */
    }
}