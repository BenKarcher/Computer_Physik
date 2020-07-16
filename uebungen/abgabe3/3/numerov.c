#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void numerovStep(double (*g)(double, double *), double (*s)(double, double *), double *x, double h, double y[2], double lambda, double *args)
{
    double c1 = 1.0 + h * h / 12 * (lambda - g(*x + h, args));
    double c2 = 2.0 * (1.0 - 5.0 * h * h / 12.0 * (lambda - g(*x, args)));
    double c3 = 1.0 + h * h / 12.0 * (lambda - g(*x - h, args));
    double c4 = h * h / 12.0 * (s(*x + h, args) + 10.0 * s(*x, args) + s(*x - h, args));
    double temp = y[1];
    y[1] = (y[1] * c2 - y[0] * c3 + c4) / c1;
    y[0] = temp;
    *x += h;
}

struct numerov
{
    double (*g)(double, double *);
    double (*s)(double, double *);
    double x0;
    double u0;
    double x1;
    double u1;
    double h;
    double *args;
};

//Sekantenverfahren auf f mit den Startwerten x1,x2 anwenden.
double secant(double x1, double x2, double (*f)(double, struct numerov*), struct numerov *args)
{
    double xn; //Speicher für den nächsten Schritt
    do
    {
        xn = x2 - f(x2, args) * (x2 - x1) / (f(x2, args) - f(x1, args));
        x1 = x2;
        x2 = xn;
    } while (fabs(x2 - x1) > 1e-6); //Bis die Schrittweite <= 1e-6
    return x2;
}

double numerovEnd(double lambda, struct numerov *args)
{
    double y[2]={args->u0,args->u0};
    double x = args->x0;
    for (double x = args->x0; x < args->x1; numerovStep(args->g, args->s, &x, args->h, y, lambda, args->args));
    return y[1]-(args->u1);
}

double firstZero(double (*f)(double,struct numerov*), double start, double step, struct numerov *args){
    double next, last;
    next = last = f(start,args);
    while(last*next>0){
        start+=step;
        last=next;
        next=f(start,args);
    }
    return secant(start-step,start,f,args);
}

double g(double x, double *args)
{
    return 0;
}

double s(double x, double *args)
{
    return 0;
}


void main()
{
    double h = 0.0005;
    double y[2] = {1.0, 1.0};
    double args[] = {1.0};
    struct numerov numerovArgs = {g, s, 0, 1, 2*M_PI*10, 0, 0.001, args};

    double start = -1;
    double nst = 0;
    for (int i = 0; i < 10; i++)
    {
        nst = firstZero(numerovEnd, start, h, &numerovArgs);
        //printf("Nullstelle %d bei %lf\n",i+1,nst);
        start = nst+h;
    }
    
    for (double x = 0; x<60; numerovStep(g,s,&x,h,y,nst,args))
    {
        printf("%lf %lf\n",x,y[1]);
    }
    
    
    /*for (double lambda=0; lambda<10; lambda+=0.0001)
    {
        printf("%lf %lf\n",lambda,numerovEnd(lambda,&numerovArgs));
    }*/
}
//cos(sqrt(lambda)*x)