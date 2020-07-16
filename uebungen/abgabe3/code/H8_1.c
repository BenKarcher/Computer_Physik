//gcc -Wall H8_1.c numerov.c -o abgabe.out -lm
//./abgabe.out
//Ben Karcher, Annika Hoverath
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "numerov.h"

#define numValues 24
#define gapSize 5.0

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
    FILE *file = safeOpen("8_1_wellen");
    double args[] = {};
    double L = 8;
    struct numerov numerovArgs = {g, s, 0, 0, L, 0, 0.005, args};

    //find eigenvalues of the system
    double eigenValues[numValues];
    double lambda = 0; 
    double lambdaStep=0.001;
    int i = 0;
    while (lambda < 60) //max(Vper) = 60
    {
        lambda = firstZero(numerovError, lambda, lambdaStep, 60, &numerovArgs); //determines next zero
        if (isnan(lambda))//first zero returns nan if no zero is found before specified end
            break;
        if(i<numValues)
            eigenValues[i]=lambda;
        printf("Eigenwert %d bei %lf\n", ++i, lambda);
        lambda += lambdaStep;//do one increment manualy to prevent repeated values
    }
    
    for(i=0;i<23;i++){//find first band gap
        if(fabs(eigenValues[i]-eigenValues[i+1])>gapSize)
            break;
    }
    printf("erste bandluecke zwichen %lf, und %lf\n",eigenValues[i],eigenValues[i+1]);

    //determine norming factor for function above and below gap
    double norms[]={0,0};
    for (int j=0;j<=1;j++){
        double y[2] = {numerovArgs.u0, numerovArgs.u0 + numerovArgs.h};
        for (double x = numerovArgs.x0; x < numerovArgs.x1; numerovStep(numerovArgs.g, numerovArgs.s, &x, numerovArgs.h, y, -2*eigenValues[i+j], numerovArgs.args))
        {
            norms[j] += y[0] * y[0] * numerovArgs.h;
        }
    }
    
    //plot normed funktions
    double y1[]={numerovArgs.u0, numerovArgs.u0 + numerovArgs.h};
    double y2[]={numerovArgs.u0, numerovArgs.u0 + numerovArgs.h};
    double x2=numerovArgs.x0;
    printf("%lf %lf\n",eigenValues[i],eigenValues[i+1]);
    for (double x1 = numerovArgs.x0; x1 < numerovArgs.x1;)
    {
        numerovStep(numerovArgs.g, numerovArgs.s, &x1, numerovArgs.h, y1, -2*eigenValues[i], numerovArgs.args);
        numerovStep(numerovArgs.g, numerovArgs.s, &x2, numerovArgs.h, y2, -2*eigenValues[i+1], numerovArgs.args);
        fprintf(file, "%lf %lf %lf\n", x1, y1[0]/sqrt(norms[0]),y2[0]/sqrt(norms[1]));
    }
}