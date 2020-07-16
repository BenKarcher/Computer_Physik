#include "fft.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

using namespace std;

//constants for signal
double tmax = 5, tint = 100., tL = 50;
double alpha0 = 10, alpha1 = 0;
double beta = 1;
double a = 0, b = 0;

complex<double> signal(double t)
{
    return (0 <= t && t <= tmax) ? sin(2 * M_PI * (alpha0 + t * alpha1) * t) : 0;
}

complex<double> echo(double t)
{
    return signal(t - tL) + (2.0 * a * random() / RAND_MAX) - a + b * sin(2 * beta * t);
}

complex<double> korrelation(double t)
{
    if (t < tL - tmax) {
        return 0;
    } else if (tL - tmax < t && t <= tL) {
        return 0.5 * cos(2. * M_PI * alpha0 * (t - tL)) * (tmax - tL + t) - 1. / (4. * M_PI * alpha0 * tmax) * cos(2. * M_PI * alpha0 * tmax) * sin(2. * M_PI * alpha0 * (tmax - tL + t));
    } else if (tL < t && t < tL + tmax) {
        return 0.5 * cos(2. * M_PI * alpha0 * (t - tL)) * (tL - t + tmax) - 1. / (4. * M_PI * alpha0 * tmax) * cos(2. * M_PI * alpha0 * tmax) * sin(2. * M_PI * alpha0 * (tmax + tL - t));
    } else {
        return 0;
    }
}

int main()
{
    discrete_function s(signal, 13, tint);
    discrete_function e(echo, 13, tint);
    discrete_function k(korrelation, 13, tint);
    char filename[50];

    //aufgabe 2
    system("mkdir -p ./plots/echo");
    for (a=b=0.5; a<=8; a=b*=2)
    {
        sprintf(filename,"./plots/echo/%.1lf.plot",a);
        e.reset().norm().plot(filename);
    }

    //aufgabe 3
    system("mkdir -p ./plots/korrelation");
    for (a=b=0.5; a<=8; a=b*=2)
    {
        sprintf(filename,"./plots/korrelation/%.1lf.plot",a);
        s.reset();
        e.reset();
        (s.fft() *= e.fft().conj()).rfft().norm().plot(filename);
    }
    
    //aufgabe 4
    system("mkdir -p ./plots/analytisch");
    sprintf(filename,"./plots/analytisch/numerisch.plot");
    a=b=0;
    s.reset();
    e.reset();
    (s.fft() *= e.fft().conj()).rfft().norm().plot(filename);
    sprintf(filename,"./plots/analytisch/analytisch.plot");
    k.norm().plot(filename);

    //augabe 5
    alpha0=5.0;
    alpha1=1.0;
    system("mkdir -p ./plots/zwitscher");
    for (a=b=0.5; a<=8; a=b*=2)
    {
        sprintf(filename,"./plots/zwitscher/%.1lf.plot",a);
        s.reset();
        e.reset();
        (s.fft() *= e.fft().conj()).rfft().norm().plot(filename);
    }
    return 0;
}