//gcc -Wall abgabe2_4.c diffEqu.c -o abgabe.out -lm
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

    double y[2] = {y0,v0};
    double dy_dt[2];
    double args[2] = {w,g};
    FILE *fout=safeOpen("euler");
    fprintf(fout, "t X_{euler} X_{exact} Differenz\n");
    for (double t = t0; t <= tend; t += h)
    {
        double tmp = creal(exact_solution(t, v0, y0,w,g));
        fprintf(fout, "%20.5le %20.5le %20.5le %20.5le\n", t, y[0], tmp, y[0] - tmp);

        euler_step(2, 2, h, t, y, dy_dt, args,diff_harmonic);
    }

    double k[4][2];
    double *kp[4]={k[0],k[1],k[2],k[3]};//Pointers to the subbarays of k
    y[0]=y0;
    y[1]=v0;
    fout=safeOpen("RuKu4");
    fprintf(fout, "t X_{RuKu4} X_{exact} Differenz\n");
    for (double t = t0; t <= tend; t += h)
    {
        double tmp = creal(exact_solution(t, v0, y0,w,g));
        fprintf(fout, "%20.5le %20.5le %20.5le %20.5le\n", t, y[0], tmp, y[0] - tmp);

        RuKu4_step(2, 2, h, t, y, dy_dt,kp, args,diff_harmonic); /* Dgl.schritt ausfuehren */
    }
}