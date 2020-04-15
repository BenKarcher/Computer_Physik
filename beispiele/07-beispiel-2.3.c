/* Datei: beispiel-2.3.c   Datum: 16.4.2016 */  

...

double x,y;

...

/* if-Anweisung */  
if(fabs(x)<0.3)        /* Bedingung */  
  {
    y = 1+x+0.5*pow(x,2);    /* Block wird ausgefuehrt falls Bedingung "wahr" */
  }
 else
  {
    y=exp(x);     /* Block wird ausgefuehrt falls Bedingung "falsch" */
  }

...


