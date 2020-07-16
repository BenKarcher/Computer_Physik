//gcc -Wall H7.c numerov.c -o abgabe.out -lm
//./abgabe.out
//Ben Karcher, Annika Hoverath
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "numerov.h"

double g(double x, double *args)
{
    return 0;
}

double s(double x, double *args)
{
    return 0;
}

int main(void)
{
    FILE *fileAmplitude = safeOpen("7_Amplitude");
    FILE *fileEndValue = safeOpen("7_EndValue");
    double lambdaStep = 0.00005;
    double args[] = {1.0};
    struct numerov numerovArgs = {g, s, 0, 1, 60, 0, 0.001, args};

    double lastAmplitude = 0;
    double difference = 0;
    int eigenValue=1;

    for (double lambda = 0.01; lambda > -0.3; lambda -= lambdaStep) //negative Eigenwerte -> Exponentialfunktion: keine Amplitude
    {
        double endValue=numerovBackError(lambda, &numerovArgs) + numerovArgs.u0;//determines end value when starting with slope=1
        double bestSlope=numerovArgs.u0 / endValue;//determines best slope by scaling function to reach desired output.

        //determine amplitude of solution
        double amplitude = 0;
        double y[2] = {numerovArgs.u1, numerovArgs.u1 + (numerovArgs.h * bestSlope)};//start solution with desired slope and value
        for (double x = numerovArgs.x1; x > numerovArgs.x0; numerovBackStep(numerovArgs.g, numerovArgs.s, &x, numerovArgs.h, y, lambda, numerovArgs.args))
        {
            if (fabs(y[0]) > amplitude)
            {
                amplitude = fabs(y[0]);
            }
        }

        fprintf(fileAmplitude,"%lf %lf\n",lambda,amplitude);
        fprintf(fileEndValue,"%lf %lf\n",lambda,endValue);
        if (difference > 0 && (amplitude - lastAmplitude) < 0)
        {
            double exact=-pow(eigenValue*M_PI/60.0,2.0);
            printf("Eigenwert %d num: %lf exact: %lf diff: %lf\n", eigenValue,lambda,exact,lambda-exact);
            eigenValue++;
        }
        difference=amplitude - lastAmplitude;
        lastAmplitude = amplitude;
    }
    return 0;
}
