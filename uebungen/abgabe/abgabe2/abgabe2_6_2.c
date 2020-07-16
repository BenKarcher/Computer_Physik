//gcc -Wall abgabe2_6_2.c diffEqu.c -o abgabe.out -lm -L. -lh2_cip
//./abgabe.out
//Ben Karcher, Annika Hoverath

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "diffEqu.h"

#define perioden 12
int main(void)
{
    double y0 = 1.5;
    double v0 = 2.75;
    double w = 1;
    double g = 0.1;
    double h = 0.0001;

    double y[2] = {y0, v0};
    double dy_dt[2];
    double args[2] = {w, g};
    
    double k1[2],k2[2],k3[2],k4[2];
    double *kp[16] = {k1, k2, k3, k4};

    FILE *fout = safeOpen("fext_plot");
    fprintf(fout, "t x v fext leistung\n");
    int schritte = 0;
    int sign = 1;
    double average[perioden+1] = {0};
    int periode = 0;
    for (double t = 0; periode <= perioden; t += h)
    {
        schritte++;
        if (sign * y[0] < 0)
        {
            sign = -sign;
            if (sign < 0)
            {   
                average[periode] /= schritte;
                periode++;      
                schritte = 0;
            }
        }
        average[periode] += y[1] * fext(t);
        fprintf(fout, "%le %le %le %le %le\n", t, y[0], y[1], fext(t), y[1] * fext(t));

        RuKu4_step(2, 4, h, t, y, dy_dt, kp, args, diff_unkown_harmonic); /* Dgl.schritt ausfuehren */
    }
    fout=safeOpen("fext_leistung");
    fprintf(fout, "t leistung\n");
    for (int i = 1; i <= perioden; i++)
    {
        fprintf(fout, "%d %le\n", i, average[i]);
    }
}
