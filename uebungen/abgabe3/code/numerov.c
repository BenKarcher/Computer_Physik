//Dieser file wird von anderen aufgerufen und muss nicht einzel ausgefuert werden
//Ben Karcher, Annika Hoverath
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "numerov.h"

//does one numerov step of the form u"(x)=(lambda-g(x))u(x)+s(x)
void numerovStep(double (*g)(double, double *), double (*s)(double, double *), double *x, double h, double y[2], double lambda, double *args)
{
    double c1 = 1.0 + h * h / 12 * (g(*x + h, args) - lambda);//calculates coeficients based on formula from wikipedia
    double c2 = 2.0 * (1.0 - 5.0 * h * h / 12.0 * (g(*x, args) - lambda));
    double c3 = 1.0 + h * h / 12.0 * (g(*x - h, args) - lambda);
    double c4 = h * h / 12.0 * (s(*x + h, args) + 10.0 * s(*x, args) + s(*x - h, args));
    double temp = y[1];
    y[1] = (y[1] * c2 - y[0] * c3 + c4) / c1;//computes next value
    y[0] = temp;
    *x += h;
}
//does one backward numerov step of the form u"(x)=(lambda-g(x))u(x)+s(x)
void numerovBackStep(double (*g)(double, double *), double (*s)(double, double *), double *x, double h, double y[2], double lambda, double *args)
{
    double c1 = 1.0 + h * h / 12 * (g(*x + h, args) - lambda);//calculates coeficients based on formula from wikipedia
    double c2 = 2.0 * (1.0 - 5.0 * h * h / 12.0 * (g(*x, args) - lambda));
    double c3 = 1.0 + h * h / 12.0 * (g(*x - h, args) - lambda);
    double c4 = h * h / 12.0 * (s(*x + h, args) + 10.0 * s(*x, args) + s(*x - h, args));
    double temp = y[0];
    y[0] = (y[0] * c2 - y[1] * c1 + c4) / c3;//computes next value
    y[1] = temp;
    *x -= h;
}

//calculates how far the right edge condition was missed when starting with a slope of 1
double numerovError(double lambda, struct numerov *args)
{
    double y[2] = {args->u0 - args->h, args->u0};
    for (double x = args->x0; x < args->x1; numerovStep(args->g, args->s, &x, args->h, y, -2 * lambda, args->args))
        ;
    return y[1] - (args->u1);
}

//calculates how far the left edge condition was missed when starting with a slope of 1 and going backwards
double numerovBackError(double lambda, struct numerov *args)
{
    double y[2] = {args->u1, args->u1 + args->h};
    for (double x = args->x1; x > args->x0; numerovBackStep(args->g, args->s, &x, args->h, y, lambda, args->args))
        ;
    return y[0] - (args->u0);
}

//narrows down the zero point of the funktion
//used inplace of secant because it converges better for exponential funktions
double binarySearch(double x1, double x2, double (*f)(double, struct numerov *), struct numerov *args)
{
    double f1 = f(x1, args);
    double xn;//middle value
    while (fabs(x1 - x2) > 1e-6)
    {
        xn = (x1+x2)/2;//new midpoint
        if (f(xn, args)*f1>0)
        {
            x1 = xn;
        }
        else
        {
            x2 = xn;
        }
    }
    return (x1 + x2) / 2;
}

//scans roughly for the first zero pass and then calls binary search to narrow down range
double firstZero(double (*f)(double, struct numerov *), double start, double step, double end, struct numerov *args)
{
    double next, last;
    next = last = f(start, args);
    while (last * next > 0)//checks for oposite signs
    {
        if (start > end)
        {
            return NAN;
        }
        start += step;
        last = next;
        next = f(start, args);
        //printf("%lf %lf\n",start,next);
    }
    return binarySearch(start - step, start, f, args);
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