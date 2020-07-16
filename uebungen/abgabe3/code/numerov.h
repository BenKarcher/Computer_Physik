//Dieser file wird von anderen aufgerufen und muss nicht einzel ausgefuert werden
//Ben Karcher, Annika Hoverath
#ifndef NUMEROV_H_INCLUDED
#define NUMEROV_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "numerov.h"

struct numerov//contains all information necissary to execute numerov
{
    double (*g)(double, double *);
    double (*s)(double, double *);
    double x0;//boundary conditions
    double u0;
    double x1;
    double u1;
    double h;//step size
    double *args;//arguments to g and s
};

//does one numerov step of the form u"(x)=(lambda-g(x))u(x)+s(x)
void numerovStep(double (*g)(double, double *), double (*s)(double, double *), double *x, double h, double y[2], double lambda, double *args);

//does one backward numerov step of the form u"(x)=(lambda-g(x))u(x)+s(x)
void numerovBackStep(double (*g)(double, double *), double (*s)(double, double *), double *x, double h, double y[2], double lambda, double *args);

//calculates how far the right edge condition was missed when starting with a slope of 1
double numerovError(double lambda, struct numerov *args);

//calculates how far the left edge condition was missed when starting with a slope of 1 and going backwards
double numerovBackError(double lambda, struct numerov *args);

//narrows down the zero point of the funktion
//used inplace of secant because it converges better for exponential funktions
double binarySearch(double x1, double x2, double (*f)(double, struct numerov *), struct numerov *args);

//scans roughly for the first zero pass and then calls binary search to narrow down range
double firstZero(double (*f)(double, struct numerov *), double start, double step, double end, struct numerov *args);

FILE *safeOpen(const char *name);

#endif //NUMEROV
