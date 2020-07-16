//gcc -Wall abgabe2_6_3.c diffEqu.c -o abgabe.out -lm -L. -lh2_cip
//./abgabe.out
//Ben Karcher, Annika Hoverath

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "diffEqu.h"

#define perioden 60
#define scanNum 300

int main(void)
{
    //H4
    /* Eingabe der Parameter */
    double t0 = 0;
    double h = 0.01;
    //printf("Bitte geben Sie t_0,t_end,h ein \n");
    //scanf(" %le %le %le", &t0, &tend, &h);
    //Parameter aus Aufagbenstellung
 
    FILE *fout=safeOpen("fext_scan");
    fprintf(fout, "w leistung -1std +1std\n");
    double y[2] = {1.5, 2.75};
    double *y0=y;
    double *v0=y+1;
    double dy_dt[2];
    double args[2] = {1.0, 0.1};
    double *w = args;

    double k[4][2];
    double *kp[4] = {k[0], k[1], k[2], k[3]}; //Pointers to the subbarays of k
    double *test=kp[2];
    int steps = 0;
    int sign = 1;
    
    for (int scan=0; scan<scanNum; scan++)
    {
        *w=2.0*((double)scan/scanNum)+1;
        double average=0;
        int periode =0;
        *y0=1.5;
        *v0=2.75;
        double averagePeriode[perioden+1] = {0};
        for (double t = t0; periode <= perioden; t += h)
        {
            steps++;
            if (sign * y[0] < 0)
            {
                sign = -sign;
                if (sign < 0)
                {
                    averagePeriode[periode]/=steps;
                    average+=averagePeriode[periode];
                    periode++;
                    steps = 0;
                }
            }
            averagePeriode[periode] += y[1] * fext(t);
            RuKu4_step(2, 4, h, t, y, dy_dt, kp, args, diff_unkown_harmonic); /* Dgl.schritt ausfuehren */
        }
        average/=perioden;
        double variance=0;
        for (int i = 0; i < perioden; i++)
        {
            variance += (average-averagePeriode[i])*(average-averagePeriode[i]);
        }
        variance/=perioden;
        fprintf(fout, "%le %le %le %le\n", *w, average, average-variance, average+variance);
    }
}
