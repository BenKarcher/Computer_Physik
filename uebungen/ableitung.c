#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double ableitung(double (*f)(double),double x, double h);
double n_ableitung(int n, double (*f)(double),double x, double h);
int factorial(int x);
int factorial2(int x);
int choose(int n, int k);

/* Funktion "main" wird bei Start des Programms ausgefuehrt */ 
int main()   /* int definiert den Datentyp der Groesse, 
                die "main" zurueck gibt */ 
{            /* geschweifte Klammern begrenzen Programmbloecke
                hier werden alle Anweisungen der Funktion 
                eingefasst */   
  printf("x= %2.5lf \n",n_ableitung(2,sin,2,0.00001));
  
  return 0;                       /* Rueckgabe einer 0 an die aufrufende 
				     Funktion (Betriebssytem) */ 
}

double ableitung(double (*f)(double),double x, double h){
    return (f(x+h)-f(x-h))/(2*h);
}

double n_ableitung(int n, double (*f)(double),double x, double h){
    double sum=0;
    double power= 1.0/pow(2.0*h, n);
    for(int i=0; i<=n; i++){
        sum += (1-2*(i%2)) * f(x+h*(n-2*i)) * choose(n,i) * power;
    }
    printf("sum=%2.20lf",sum);
    return sum ;
}

int factorial(int x){
    return x?x*factorial(x-1):1;
}

int factorial2(int x){
	if (x != 0) {
		int product = 1;
		for(int i=x; i>=1; i--) {
			product *= i;
		}
		return product;
	} else {
		return 1;
	}
}

int choose(int n, int k){
    return k>n?0:factorial(n)/(factorial(k)*factorial(n-k));
}