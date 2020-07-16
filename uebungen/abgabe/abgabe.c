// gcc -lm abgabe.c -o abgabe.out -lm
// ./abgabe.out
// Ben Karcher Annika Hoverath

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Max_steps 10
#define PI 3.14159265358979323846

//aditional arguments are passed as arrays so that derivation and integration works genericaly

double integrant(double x, double *z_a);                  //e^(-x^2/a^2)/sqrt(z^2+x^2)
double potential(double z, double *a_q);                  //Calculates the potential at point z by integrating the integrant
double e_Field(double z, double *a_q);                    //Calculates the z component of the Electric field by finding the z derivative of the potential
double dz_integrant(double x, double *z_a);               //the derivative of integrant with respect to z
double e_Field2(double z, double *a_q);                   //Calculates the z component of the Electric field by integrating dz_integrant
double combined_Potential(double z, double *a1_q1_a2_q2); //Calculates the potential of a two wire system at point z

double derive(double (*f)(double x, double *args), double *args, double x, double h);
double integrate(double (*f)(double x, double *args), double *args, double x1, double x2, int n);
double romberg(double (*f)(double x, double *args), double *args, double x1, double x2, int n, double tolerance);
int hash(int i, int k); //defines a triangular hashmap for
double extrema(double (*f)(double x, double *args), double *args, double x1, double x2);

int main(void)
{
    //problem 1 prints the potential at 20000 values of z and 6 values of a into a file.
    FILE *outputP;
    outputP = fopen("potential1d16", "w");
    if (outputP == NULL)
        exit(EXIT_FAILURE);
    for (double z = 0.00005; z < 2; z += 0.0001)
    {
        //fprintf(outputP, "%lf", z);
        //for (double a_q[] = {4, 1}; a_q[0] > 0.003; a_q[0] /= 4)
        //{
            double a_q[] = {1.0/16.0, 1};
            fprintf(outputP, "%lf %lf\n", z,potential(z, a_q));
        //}
        //fprintf(outputP, "\n");
    }

    //problem 2 calculates the value of E_z using two methods and prints the maximum error.
    double max_error = 0;
    double max_percent_error = 0;
    for (double z = 2; z > 0.003; z /= 2)
    {
        for (double a_q[] = {4, 1}; a_q[0] > 0.003; a_q[0] /= 2)
        {
            double e1 = e_Field(z, a_q);
            double e2 = e_Field2(z, a_q);
            if (fabs(e1 - e2) > max_error)
            {
                max_error = fabs(e1 - e2);
            }
            if (fabs(2 * (e1 - e2) / (e1 + e2)) > max_error)
            {
                max_percent_error = fabs(2 * (e1 - e2) / (e1 + e2));
            }
        }
    }
    printf("The max percent error between the two methods was %lf%%\n", max_percent_error * 100);
    printf("The max absolute error between the two methods was %lf\n", max_error);

    //calculates the equilibrium point in a two wire system using a extreme value of the potential
    for (double a2 = 4; a2 > 0.4; a2 /= 2)
    {
        double a1_q1_a2_q2[] = {4, 1, a2, 4};
        printf("with a1=4, q1=1, a2=%lf, q2=4 The equilibrium point is a z=%lf\n", a1_q1_a2_q2[2], extrema(combined_Potential, a1_q1_a2_q2, 0.001, 11.999));
    }
}

double integrant(double x, double *z_a)
{
    return exp(-(x * x) / (z_a[1] * z_a[1])) / sqrt((x * x) + (z_a[0] * z_a[0]));
}

double potential(double z, double *a_q)
{
    double args[] = {z, a_q[0]};
    return a_q[1] / a_q[0] / sqrt(PI) * romberg(integrant, args, -10 * a_q[0], 10 * a_q[0], 50, 1e-8);
}

double e_Field(double z, double *a_q)
{
    return -derive(potential, a_q, z, 0.001);
}

double dz_integrant(double x, double *z_a)
{
    return -z_a[0] * exp(-(x * x) / (z_a[1] * z_a[1])) / pow(x * x + z_a[0] * z_a[0], 1.5);
}

double e_Field2(double z, double *a_q)      //since the integral is with respect to x the d/dz can be pulled in as done here
{
    double args[] = {z, a_q[0]};
    return -a_q[1] / a_q[0] / sqrt(PI) * romberg(dz_integrant, args, -10 * a_q[0], 10 * a_q[0], 50, 1e-8);
}

double combined_Potential(double z, double *a1_q1_a2_q2)
{
    return potential(z, a1_q1_a2_q2) + potential(12 - z, a1_q1_a2_q2 + 2);//by incrementing the pointer by 2 the second set of values is passed for the second wire
}

double derive(double (*f)(double x, double *args), double *args, double x, double h)
{
    return (f(x + h, args) - f(x - h, args)) / (2 * h);
}

double integrate(double (*f)(double x, double *args), double *args, double x1, double x2, int n)// uses the trapezoid method
{
    double h = (x2 - x1) / n;           //width of each trapezoid
    double right, left = f(x1, args);   //the left and right value of the current trapezoid.
    double sum = 0;
    for (int i = 1; i <= n; i++)
    {
        right = f(x1 + h * i, args);
        sum += (left + right) * h / 2;
        left = right;
    }
    return sum;
}

int hash(int i, int k)//defines a triangular hashmap over i=[0,Maxsteps] k=[0,i]
{
    return i + k * Max_steps - k * (k - 1) / 2;
}

double romberg(double (*f)(double x, double *args), double *args, double x1, double x2, int n, double tolerance)
{
    double result;
    double h[Max_steps + 1];
    double partial_Integrals[Max_steps + 1];
    double Ttilde[(Max_steps + 1) * (Max_steps + 2) / 2];

    h[0] = (x2 - x1) / (double)(n - 1);                   //starting width
    partial_Integrals[0] = integrate(f, args, x1, x2, n); //starting value
    Ttilde[hash(0, 0)] = partial_Integrals[0];

    for (int j = 1; j <= Max_steps; j++)    //take up to Max_steps to reach the desired precission
    {
        h[j] = h[j - 1] / 2; //1/2 distance
        n = 2 * n;           //double step amount
        partial_Integrals[j] = integrate(f, args, x1, x2, n);

        Ttilde[hash(j, 0)] = partial_Integrals[j];

        for (int k = 1; k <= j; k++)
        { //calculates the next aproximation using the formula derived in the lecture
            Ttilde[hash(j, k)] = -h[j - k] * h[j - k] / (h[j] * h[j] - h[j - k] * h[j - k]) * Ttilde[hash(j, k - 1)] +
                                 h[j] * h[j] / (h[j] * h[j] - h[j - k] * h[j - k]) * Ttilde[hash(j - 1, k - 1)];
        }

        result = Ttilde[hash(j, j)];
        if (fabs(result - Ttilde[hash(j - 1, j - 1)]) <= tolerance)//checks if result is within tolerance
        {
            return result;
        }
    }
    //printf("The deisred tolerance of %lf could not be reached in %d steps.\n", tolerance,Max_steps);
    return result;
}

double extrema(double (*f)(double x, double *args), double *args, double x1, double x2)
{
    double x3; //the next aproximation
    do
    {
        x3 = x2 - derive(f, args, x2, 0.01) * (x2 - x1) / (derive(f, args, x2, 0.01) - derive(f, args, x1, 0.01)); //calculates the x intersect of a secant through f'(x1) and f'(x2)
        x1 = x2;
        x2 = x3;
    } while (fabs(x2 - x1) > 1e-6); //terminates once the interval has been reduced to 1e-6
    return x2;
}