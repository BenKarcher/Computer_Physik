/*
 *  numerov.c
 *
 *	Title:			Numerov Method
 *	Author:			Tristan Jehan
 *	Description:	Numerov method is used to integrate a simple harmonic oscillator y: + y = 0
 *					The solution for this Ordinary Differential Equation is y = cos(t)
 *					From Taylor we get y: = (yn+1 -2*yn + yn-1) / h^2
 *					So  (yn+1 -2*yn + yn-1) / h^2 + yn = 0 is our equation
 *					It gives us yn+1 = 2*yn - yn-1 - h*h * yn 
 *					with ydot0 = (y1 - y0)/h = 0
 *					y1 = y0 = 1
 *	Date:			03/07/00	
 *	
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* f = dy/dt */
#define y(t) cos(t)	
#define PI 3.14159265359879323846f
#define TMAX 100*PI

int main()
{
	int time0,time1;
	int numIter;
	double t=0.0, h;
	double error=0, sumError=0, slope;
	double Y[3];
	char filename[21];
	FILE* myfile;
		
	/* create an output file */
	printf("Enter the name of your output file: ");
	scanf("%20s", filename);

	/* enter argument */
	printf("Enter the step size: ");
	scanf("%lf", &h);
		
	/* check out for arguments */
	if ((h<=0) || (h>TMAX)) {
		printf("Hey, you gave a wrong step size!");
		exit(0);
	}

	numIter = (int)(TMAX/h);

	/* write in file */
	myfile = fopen(filename,"w");
	fprintf(myfile,"%d\n%lf\n%f\n", numIter, h, TMAX);
		
	Y[0] = 1;
	Y[1] = 1;
	/* main loop */
	while(t<TMAX) {
		Y[2] = 2.0f * Y[1] - Y[0] - h*h * Y[1]; 
		Y[0] = Y[1];
		Y[1] = Y[2];
		t+=h;
		error = fabs(y(t) - Y[1]);
		sumError += error;
		fprintf(myfile,"%lf\n", Y[2]);
	}
	slope = (Y[1]-Y[0])/h;

	/* measure time performances only */
	time0=clock();
	t=0.0;
	Y[0] = 1;
	Y[1] = 1;
	while(t<TMAX) {
		Y[2] = 2.0f * Y[1] - Y[0] - h*h * Y[1]; 
		Y[0] = Y[1];
		Y[1] = Y[2];
		t+=h;
	}
	time1=clock();
	
	/* print results */
	printf("\nThe program has computed %d points\n", numIter);
	printf("It was executed in %.3f secondes\n", (float)(time1-time0)/CLOCKS_PER_SEC);
	printf("average error: %.6f\nfinal error: %lf\nfinal slope: %lf\n", sumError/numIter, error, slope);
			
	fclose(myfile);
	return 0;
}
