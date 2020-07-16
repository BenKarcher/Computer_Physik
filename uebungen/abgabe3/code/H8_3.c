//gcc -Wall H8_3.c numerov.c -o abgabe.out -lm
//./abgabe.out
//Ben Karcher, Annika Hoverath
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "numerov.h"

#define steps 200//der plot im pdf wurde mit steps=200 gemacht dies dauert aber ziemlich lange
#define numValues 24
#define gapSize 5.0
#define numGaps 2

double v(double x, double *args)
{
    return 60.0 * pow(cos(M_PI * x), 16.0);
}
double ve(double x, double *args)
{
    return x * args[0];
}
double g(double x, double *args)
{
    return -2 * (v(x, args) + ve(x, args));
}

double s(double x, double *args)
{
    return 0;
}

double avg(double array[], int length){
    double average=0;
    for(int i=0; i<length; i++)
        average+=array[i]/length;
    return average;
}
double lineSlope(double xArr[], double yArr[], double xbar, double ybar, int length){
    double c1=0, c2=0;
    for(int i=0; i<length; i++){
        c1+= (xArr[i]-xbar)*(yArr[i]-ybar);
        c2+= (xArr[i]-xbar)*(xArr[i]-xbar);
    }
    return c1/c2;
}

int main(void)
{
    double start = 0;
    double lambda;
    double lambdaStep=0.03;
    double end = 60;
    FILE *file=safeOpen("8_3_epsilon");
    double epsilon[]={0.0};
    struct numerov numerovArgs = {g, s, 0, 0, 8, 0, 0.05, epsilon};

    double edges[2*numGaps][steps];//points for the linear regression
    double xArray[steps];
    double eigenValues[numValues];
    int j,k,gap;
    for (int i=0; i<steps;i++)
    {
        *epsilon=(double) i/(steps-1);
        lambda = start;
        j=0;
        while (lambda < end)
        {
            lambda = firstZero(numerovError, lambda, lambdaStep, end, &numerovArgs);
            if (isnan(lambda))
                break;
            if(j<numValues)
                eigenValues[j]=lambda;
            j++;
            fprintf(file, "%lf %lf\n", *epsilon, lambda);
            lambda += lambdaStep;
        }
        //find gaps
        xArray[i]=*epsilon;
        k=0;
        for(gap=0;gap<numGaps;gap++){
            for(; k<j;k++){
                if(fabs(eigenValues[k]-eigenValues[k+1])>gapSize)
                    break;
            }
            edges[2*gap][i]=eigenValues[k];
            edges[2*gap+1][i]=eigenValues[k+1];
            k++;
        }
    }
    //linear regression on gaps
    double xbar=avg(xArray, steps);
    for(int i=0; i<numGaps; i++){
        double yBarBot=avg(edges[2*i],steps);
        double yBarTop=avg(edges[2*i+1],steps);
        double slopeBot=lineSlope(xArray,edges[2*i],xbar,yBarBot,steps);
        double slopeTop=lineSlope(xArray,edges[2*i+1],xbar,yBarTop,steps);
        double interceptBot=yBarBot-slopeBot*xbar;
        double interceptTop=yBarTop-slopeTop*xbar;
        printf("band gap %d is bounded by λ= %0.2lfε + %0.2lf and λ= %0.2lfε + %0.2lf\n",i+1,slopeBot,interceptBot,slopeTop,interceptTop);
        printf("band gap %d is given by λ= %0.2lfε + %0.2lf ± (%0.2lfε + %0.2lf)\n",i+1,0.5*(slopeBot+slopeTop),0.5*(interceptBot+interceptTop),slopeTop-slopeBot,interceptTop-interceptBot);
    }
}