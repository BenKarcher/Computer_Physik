//gcc main.c -lm -o main -Wall -Wextra -g -O3
//./main
//to make plots call plot.sh this calls gnuplot and ffmpeg
//Ben Karcher, Anika Hoverath
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

// um zu schauen, ob der Ordner existiert
extern int errno;
struct stat st = {0};

//ffmpeg -i time%02d.png -vf scale="trunc(iw/2)*2:trunc(ih/2)*2" -c:v libx264 -profile:v high -pix_fmt yuv420p -g 25 -r 25 output.mp4

//derive a function in its second argument
//used to find local minima of u2
double derive(double (*f)(double, double), double x, double t, double h)
{
    return (f(t, x + h) - f(t, x - h)) / (2 * h);
}

//finds local extrema of a function between x1 and x2
//used to find local minima of u2
double findExtrema(double x1, double x2, double t, double (*func)(double, double))
{
    // x1,x2
    double f1 = derive(func, x1, t, 0.001);
    double xn;
    while (fabs(x1 - x2) > 1e-5)
    {
        xn = (x1 + x2) / 2;
        if (f1 * derive(func, xn, t, 0.001) > 0)
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

//safely accesses an array of given length returns 0 if out of bounds
double edge(double *arr, long idx, long length)
{
    return (idx < 0 || idx >= length) ? 0 : arr[idx];
}

// calculates next value at point j
double step(double *u0, double *u1, double j, double h, double d, long len)
{
    double a = (edge(u1, j + 1, len) + edge(u1, j, len) + edge(u1, j - 1, len)) * (edge(u1, j + 1, len) - edge(u1, j - 1, len)) / 6.0 / h;
    double b = (edge(u1, j + 2, len) - 2.0 * edge(u1, j + 1, len) + 2.0 * edge(u1, j - 1, len) - edge(u1, j - 2, len)) / 2.0 / (h * h * h);
    return ((6.0 * a - b) * 2 * d) + edge(u0, j, len);
}

//the start condition for N waves
double uN(double x, double N)
{
    return -N * (N + 1) / (cosh(x) * cosh(x));
}

// exact solution for N=1
double u_1(double t, double x)
{
    return -2 / (cosh(x - 4 * t) * cosh(x - 4 * t));
}

// exact solution for N=2
double u_2(double t, double x)
{
    return -12. * (3. + 4. * cosh(2. * x - 8. * t) + cosh(4. * x - 64. * t)) / pow(3. * cosh(x - 28. * t) + cosh(3. * x - 36. * t), 2.);
}

int main(void)
{
    double hWerte[] = {0.4, 0.2, 0.1, 0.05}; // Die hs
    double xmin = -30;
    double xmax = 30;
    double tmin = -1;
    double tmax = 1;
    double N = 2.0;
    char str[50];                 // string for name of file
    double *u0, *u1, *un;         // arrays of points only to are malloced un is for swaping
    unsigned int numFrames = 100; // number of frames in a video
    unsigned int frameCount = 0;  // current frame number

    // create files to save plots
    sprintf(str, "plots/"); //file for all plots
    if (stat(str, &st) == -1)
    {
        mkdir(str, 0700);
    }
    FILE *timePlot;
    sprintf(str, "plots/plotExact/"); //file for exact solution
    if (stat(str, &st) == -1)
    {
        mkdir(str, 0700);
    }

    double h = 0.1;
    long xn = ceil(fabs(xmin - xmax) / h);

    // problem 1
    for (long tc = 0; tc < numFrames; tc++)
    {
        double t = tmin + (tmax - tmin) * tc / (numFrames - 1);
        sprintf(str, "plots/plotExact/time%02u", frameCount);
        timePlot = fopen(str, "w");
        frameCount++;

        for (long xc = 0; xc < xn; xc++)
        {
            double x = xc * h + xmin;
            fprintf(timePlot, "%lf %lf\n", x, u_2(t, x));
        }
        fclose(timePlot);
    }
    double min1 = findExtrema(0., 15., 1., u_2);
    double min2 = findExtrema(15., 30., 1., u_2);
    printf("Solitionengeschwindigkeit: %lf, %lf\n", min1, min2);

    // problem 2
    sprintf(str, "plots/plotExactN1/");
    printf("%s\n", str);
    if (stat(str, &st) == -1)
    {
        mkdir(str, 0700);
    }
    for (long tc = 0; tc < numFrames; tc++)
    {
        double t = tmin + (tmax - tmin) * tc / (numFrames - 1);
        sprintf(str, "plots/plotExactN1/time%02u", frameCount);
        timePlot = fopen(str, "w");
        frameCount++;

        for (long xc = 0; xc < xn; xc++)
        {
            double x = xc * h + xmin;
            fprintf(timePlot, "%lf %lf\n", x, u_1(t, x));
        }
        fclose(timePlot);
    }
    tmin = 0;
    numFrames = 250;
    N = 1.;
    // Numeric calculation
    for (int hCount = 0; hCount < sizeof(hWerte) / sizeof(hWerte[0]); hCount++)
    {
        sprintf(str, "plots/plotN1%02d/", hCount);
        printf("%s\n", str);
        if (stat(str, &st) == -1)
        {
            mkdir(str, 0700);
        }
        frameCount = 0;
        sprintf(str, "plots/plotN1%02d/time%02ld", hCount, (long)0);
        timePlot = fopen(str, "w");
        if (timePlot == NULL)
            exit(EXIT_FAILURE);
        frameCount++;
        double h = hWerte[hCount];
        double d = 1. / 2.6 * h * h * h;
        long xn = ceil(fabs(xmin - xmax) / h);
        long tn = ceil(fabs(tmin - tmax) / d);
        u0 = (double *)malloc(sizeof(double) * xn);
        u1 = (double *)malloc(sizeof(double) * xn);

        //instantiate array using uN
        for (long xc = 0; xc < xn; xc++)
        {
            u0[xc] = uN(xmin + xc * h, N);
            fprintf(timePlot, "%lf %lf\n", xc * h + xmin, u0[xc]);
        }
        for (long xc = 0; xc < xn; xc++)//set u1 using formula as derived in pdf
        {
            u1[xc] = (step(u0, u0, xc, h, d, xn) + u0[xc]) / 2.0;
        }
        //iterate over each time step
        for (long tc = 2; tc < tn; tc++)
        {
            if (tn * frameCount < tc * numFrames)//determines when to draw a new frame
            {
                sprintf(str, "plots/plotN1%02d/time%02u", hCount, frameCount);//file name
                fclose(timePlot);
                timePlot = fopen(str, "w");
                frameCount++;
                for (long xc = 0; xc < xn; xc++)
                {
                    u0[xc] = step(u0, u1, xc, h, d, xn);
                    fprintf(timePlot, "%lf %lf\n", xc * h + xmin, u0[xc]);
                }
            }
            else
            {//does step without writing to file
                for (long xc = 0; xc < xn; xc++)
                {
                    u0[xc] = u(u0, u1, xc, h, d, xn);
                }
            }
            //swaps arrays because next u is writen into u0 each step
            un = u0;
            u0 = u1;
            u1 = un;
        }
        // free memory
        fclose(timePlot);
        free(u0);
        free(u1);
    }

    N = 2.;
    tmin = 0;
    numFrames = 250;
    //same code as above but for n=2
    for (int hCount = 0; hCount < sizeof(hWerte) / sizeof(hWerte[0]); hCount++)
    {
        sprintf(str, "plots/plot%02d/", hCount);
        printf("%s\n", str);
        if (stat(str, &st) == -1)
        {
            mkdir(str, 0700);
        }
        frameCount = 0;
        sprintf(str, "plots/plot%02d/time%02ld", hCount, (long)0);
        timePlot = fopen(str, "w");
        if (timePlot == NULL)
            exit(EXIT_FAILURE);
        frameCount++;
        double h = hWerte[hCount];
        double d = 1. / 2.6 * h * h * h;
        long xn = ceil(fabs(xmin - xmax) / h);
        long tn = ceil(fabs(tmin - tmax) / d);
        u0 = (double *)malloc(sizeof(double) * xn);
        u1 = (double *)malloc(sizeof(double) * xn);

        for (long xc = 0; xc < xn; xc++)
        {
            u0[xc] = uN(xmin + xc * h, N);
            fprintf(timePlot, "%lf %lf\n", xc * h + xmin, u0[xc]);
        }
        for (long xc = 0; xc < xn; xc++)
        {
            u1[xc] = (step(u0, u0, xc, h, d, xn) + u0[xc]) / 2.0;
        }
        for (long tc = 2; tc < tn; tc++)
        {
            if (tn * frameCount < tc * numFrames)
            {
                sprintf(str, "plots/plot%02d/time%02u", hCount, frameCount);
                fclose(timePlot);
                timePlot = fopen(str, "w");
                frameCount++;
                for (long xc = 0; xc < xn; xc++)
                {
                    u0[xc] = step(u0, u1, xc, h, d, xn);
                    fprintf(timePlot, "%lf %lf\n", xc * h + xmin, u0[xc]);
                }
            }
            else
            {
                for (long xc = 0; xc < xn; xc++)
                {
                    u0[xc] = step(u0, u1, xc, h, d, xn);
                }
            }
            un = u0;
            u0 = u1;
            u1 = un;
        }
        fclose(timePlot);
        free(u0);
        free(u1);
    }

    // Heatmap for 2
    printf("berechne heatMap für Aufgabe 2 N=1...\n");
    sprintf(str, "plots/heatMapN1");
    timePlot = fopen(str, "w");
    N = 1.0;
    xmin = -30.0;
    xmax = 30.0;
    tmin = 0;
    tmax = 1;
    double (*fExact)(double, double) = u_2;
    int maxSize = (xmax - xmin) / 0.05 + 1;
    u0 = (double *)malloc(sizeof(double) * maxSize);
    u1 = (double *)malloc(sizeof(double) * maxSize);
    for (double h = 0.05; h <= 0.4; h += 0.003)
    {
        for (double d = 0.00003; d < 0.00025; d += 0.0000025)
        {
            long xn = ceil(fabs(xmin - xmax) / h);
            long tn = ceil(fabs(tmin - tmax) / d);
            for (long xc = 0; xc < xn; xc++)
            {
                u0[xc] = uN(xmin + xc * h, N);
            }
            for (long xc = 0; xc < xn; xc++)
            {
                u1[xc] = (step(u0, u0, xc, h, d, xn) + u0[xc]) / 2.0;
            }
            double error = 0;
            for (long tc = 2; tc < tn; tc++)
            {
                double t = tmin + d * tc;
                for (long xc = 0; xc < xn; xc++)
                {
                    double x = xmin + h * xc;
                    double uNum = step(u0, u1, xc, h, d, xn);
                    u0[xc] = uNum;
                    double uExct = fExact(t, x);
                    error += fabs(uNum - uExct) / xn / tn;
                }
                un = u0;
                u0 = u1;
                u1 = un;
            }
            fprintf(timePlot, "%lf ", isnan(error) ? 1000. : error);
        }
        fprintf(timePlot, "\n");
    }

    N = 2.0;
    printf("berechne heatMap für Aufgabe 2 N=2...\n");
    sprintf(str, "plots/heatMapN2");
    timePlot = fopen(str, "w");
    for (double h = 0.05; h <= 0.4; h += 0.003)
    {
        for (double d = 0.00003; d < 0.00025; d += 0.0000025)
        {
            long xn = ceil(fabs(xmin - xmax) / h);
            long tn = ceil(fabs(tmin - tmax) / d);
            for (long xc = 0; xc < xn; xc++)
            {
                u0[xc] = uN(xmin + xc * h, N);
            }
            for (long xc = 0; xc < xn; xc++)
            {
                u1[xc] = (step(u0, u0, xc, h, d, xn) + u0[xc]) / 2.0;
            }
            double error = 0;
            for (long tc = 2; tc < tn; tc++)
            {
                double t = tmin + d * tc;
                for (long xc = 0; xc < xn; xc++)
                {
                    double x = xmin + h * xc;
                    double uNum = step(u0, u1, xc, h, d, xn);
                    u0[xc] = uNum;
                    double uExct = fExact(t, x);
                    error += fabs(uNum - uExct) / xn / tn;
                }
                un = u0;
                u0 = u1;
                u1 = un;
            }
            fprintf(timePlot, "%lf ", isnan(error) ? 1000. : error);
        }
        fprintf(timePlot, "\n");
    }
    fclose(timePlot);
    free(u0);
    free(u1);

    // Problem 3
    printf("Aufgabe 3\n");
    tmin = 0;
    numFrames = 250;
    N = 3.;
    xmax = 45;
    sprintf(str, "plots/plotN3/");
    printf("%s\n", str);
    if (stat(str, &st) == -1)
    {
        mkdir(str, 0700);
    }
    frameCount = 0;
    sprintf(str, "plots/plotN3/time%02ld", (long)0);
    timePlot = fopen(str, "w");
    if (timePlot == NULL)
        exit(EXIT_FAILURE);
    frameCount++;
    h = 0.05;
    double d = 1. / 2.6 * h * h * h;
    xn = ceil(fabs(xmin - xmax) / h);
    long tn = ceil(fabs(tmin - tmax) / d);
    u0 = (double *)malloc(sizeof(double) * xn);
    u1 = (double *)malloc(sizeof(double) * xn);

    for (long xc = 0; xc < xn; xc++)
    {
        u0[xc] = uN(xmin + xc * h, N);
        fprintf(timePlot, "%lf %lf\n", xc * h + xmin, u0[xc]);
    }
    for (long xc = 0; xc < xn; xc++)
    {
        u1[xc] = (step(u0, u0, xc, h, d, xn) + u0[xc]) / 2.0;
    }
    for (long tc = 2; tc < tn; tc++)
    {
        if (tn * frameCount < tc * numFrames)
        {
            sprintf(str, "plots/plotN3/time%02u", frameCount);
            fclose(timePlot);
            timePlot = fopen(str, "w");
            frameCount++;
            for (long xc = 0; xc < xn; xc++)
            {
                u0[xc] = step(u0, u1, xc, h, d, xn);
                fprintf(timePlot, "%lf %lf\n", xc * h + xmin, u0[xc]);
            }
        }
        else
        {
            for (long xc = 0; xc < xn; xc++)
            {
                u0[xc] = step(u0, u1, xc, h, d, xn);
            }
        }
        un = u0;
        u0 = u1;
        u1 = un;
    }
    fclose(timePlot);
    free(u0);
    free(u1);

    // Problem 4
    printf("Aufgabe 4\n");
    numFrames = 250;
    for (N = 1.; N < 2.; N += 0.1)
    {
        sprintf(str, "plots/plotN%02.1f/", N);
        printf("%s\n", str);
        if (stat(str, &st) == -1)
        {
            mkdir(str, 0700);
        }
        frameCount = 0;
        sprintf(str, "plots/plotN%02.1f/time%02ld", N, (long)0);
        timePlot = fopen(str, "w");
        if (timePlot == NULL)
            exit(EXIT_FAILURE);
        frameCount++;
        double h = 0.05;
        double d = 1. / 2.6 * h * h * h;
        long xn = ceil(fabs(xmin - xmax) / h);
        long tn = ceil(fabs(tmin - tmax) / d);
        u0 = (double *)malloc(sizeof(double) * xn);
        u1 = (double *)malloc(sizeof(double) * xn);

        for (long xc = 0; xc < xn; xc++)
        {
            u0[xc] = uN(xmin + xc * h, N);
            fprintf(timePlot, "%lf %lf\n", xc * h + xmin, u0[xc]);
        }
        for (long xc = 0; xc < xn; xc++)
        {
            u1[xc] = (step(u0, u0, xc, h, d, xn) + u0[xc]) / 2.0;
        }
        for (long tc = 2; tc < tn; tc++)
        {
            if (tn * frameCount < tc * numFrames)
            {
                sprintf(str, "plots/plotN%02.1f/time%02u", N, frameCount);
                fclose(timePlot);
                timePlot = fopen(str, "w");
                frameCount++;
                for (long xc = 0; xc < xn; xc++)
                {
                    u0[xc] = step(u0, u1, xc, h, d, xn);
                    fprintf(timePlot, "%lf %lf\n", xc * h + xmin, u0[xc]);
                }
            }
            else
            {
                for (long xc = 0; xc < xn; xc++)
                {
                    u0[xc] = step(u0, u1, xc, h, d, xn);
                }
            }
            un = u0;
            u0 = u1;
            u1 = un;
        }
        fclose(timePlot);
        free(u0);
        free(u1);
    }

    return 0;
}
