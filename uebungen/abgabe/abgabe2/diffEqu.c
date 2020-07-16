#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "diffEqu.h"

void euler_step(int nequ, int nargs, double h, double t, double y[], double dy_dt[], double args[],
                void (*diffEqu)(int nequ, int nargs, double t, double y[], double dy_dt[], double args[]))
{
    diffEqu(nequ, nargs, t, y, dy_dt, args);
    for (int i = 0; i < nequ; i++)
    {
        y[i] += h * dy_dt[i];
    }
}

void RuKu4_step(int nequ, int nargs, double h, double t, double y[], double yh[], double *k[4], double args[],
                void (*diffEqu)(int nequ, int nargs, double t, double y[], double dy_dt[], double args[]))
{
    int i;
    double h2 = 0.5 * h;
    diffEqu(nequ, nargs, t, y, k[0], args);
    for (i = 0; i < nequ; i++)
    {
        yh[i] = y[i] + h2 * k[0][i];
    }
    diffEqu(nequ, nargs, t + h2, yh, k[1], args);
    for (i = 0; i < nequ; i++)
    {
        yh[i] = y[i] + h2 * k[1][i];
    }
    diffEqu(nequ, nargs, t + h2, yh, k[2], args);
    for (i = 0; i < nequ; i++)
    {
        yh[i] = y[i] + h * k[2][i];
    }
    diffEqu(nequ, nargs, t + h, yh, k[3], args);
    for (i = 0; i < nequ; i++)
    {
        y[i] += (h2 * (k[0][i] + k[3][i]) + h * (k[1][i] + k[2][i])) / 3;
    }
}

void diff_harmonic(int nequ, int nargs, double t, double y[], double dy_dt[], double w_g[])
{
    if (nequ != 2 || nargs < 2)
    {
        printf("invalid use of diff_harmonic\n\texpected: 2 equ, 2 args\n\trecieved %d equ, %d args", nequ, nargs);
        abort();
    }
    dy_dt[0] = y[1];
    dy_dt[1] = -pow(w_g[0], 2) * y[0] - 2 * w_g[1] * y[1];
}

void diff_external_harmonic(int nequ, int nargs, double t, double y[], double dy_dt[], double w_g_wk_a[])
{
    if (nequ != 2 || nargs < 4)
    {
        printf("invalid use of diff_harmonic\n\texpected: 2 equ, 2 args\n\trecieved %d equ, %d args", nequ, nargs);
        abort();
    }
    dy_dt[0] = y[1];
    dy_dt[1] = -pow(w_g_wk_a[0], 2) * y[0] - 2 * w_g_wk_a[1] * y[1] + w_g_wk_a[3] * cos(w_g_wk_a[2] * t);
}

void diff_unkown_harmonic(int nequ, int nargs, double t, double y[], double dy_dt[], double w_g[])
{
    if (nequ != 2 || nargs < 2)
    {
        printf("invalid use of diff_harmonic\n\texpected: 2 equ, 2 args\n\trecieved %d equ, %d args", nequ, nargs);
        abort();
    }
    dy_dt[0] = y[1];
    dy_dt[1] = -pow(w_g[0], 2) * y[0] - 2 * w_g[1] * y[1] + fext(t);
}

double complex exact_solution(double t, double v0, double x0, double w, double g)
{
    double complex L1 = -g + _Complex_I * sqrt(pow(w, 2) - pow(g, 2));
    double complex L2 = -g - _Complex_I * sqrt(pow(w, 2) - pow(g, 2));
    double complex A = (x0 - _Complex_I * ((v0 + x0 * g) / sqrt(pow(w, 2) - pow(g, 2)))) / 2.0;
    double complex B = (x0 + _Complex_I * ((v0 + x0 * g) / sqrt(pow(w, 2) - pow(g, 2)))) / 2.0;

    return A * cexp(L1 * t) + B * cexp(L2 * t);
}

double complex exact_external_solution(double t, double v0, double x0, double w, double g, double wk, double a){
    double complex L1 = -g+_Complex_I*sqrt(pow(w,2)-pow(g,2));
    double complex L2 = -g-_Complex_I*sqrt(pow(w,2)-pow(g,2));

    double C = -a/(pow(wk,2)-(pow(w,2))+(4*pow(g,2)*pow(wk,2)/(pow(wk,2)-pow(w,2))));
    double D = a*2*g*wk/(pow(pow(wk,2)-(pow(w,2)),2)+(4*pow(g,2)*pow(wk,2)));

    double complex B = _Complex_I*((x0-C)*g-D*wk+v0)/(2*sqrt(pow(w,2)-pow(g,2)))+(x0-C)/2;
    double complex A = x0-B-C;

    return A*cexp(L1*t)+B*cexp(L2*t)+C*cos(wk*t)+D*sin(wk*t);
}

FILE *safeOpen(const char *name)
{
    FILE *f = fopen(name, "w");
    if (f == NULL)
    {
        printf("Error creating file");
        exit(-1);
    }
    return f;
}