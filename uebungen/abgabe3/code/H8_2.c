//gcc -Wall H8_2.c numerov.c -o abgabe.out -lm
//./abgabe.out
//Ben Karcher, Annika Hoverath
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "numerov.h"

double v(double x)
{
    return 60.0 * pow(cos(M_PI * x), 16.0);
}

double g(double x, double *args)
{
    return -2 * v(x);
}

double s(double x, double *args)
{
    return 0;
}

int main(void)
{
    FILE *file = safeOpen("8_2_L");

    double args[] = {};
    struct numerov numerovArgs = {g, s, 0, 0, 8, 0, 0.05, args};
    double *L = &numerovArgs.x1;
    for (*L = 8; *L < 128; *L *= 2)
    {
        double lambda = 0;
        double lambdaStep=0.001;
        int i=0;
        while (lambda < 60)    //max(Vper) = 60
        {
            lambda = firstZero(numerovError, lambda, lambdaStep, 60, &numerovArgs); //determines next zero
            if (isnan(lambda))                                                      //first zero returns nan if no zero is found before specified end
                break;
            fprintf(file,"%lf %lf\n", *L, lambda);
            i++;
            lambda += lambdaStep; //do one increment manualy to prevent repeated values
        }
        printf("L=%lf hat %d eigenwerte\n", *L, i);
    }
}