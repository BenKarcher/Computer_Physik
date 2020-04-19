#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Funktion "main" wird bei Start des Programms ausgefuehrt */ 
int main()   /* int definiert den Datentyp der Groesse, 
                die "main" zurueck gibt */ 
{            /* geschweifte Klammern begrenzen Programmbloecke
                hier werden alle Anweisungen der Funktion 
                eingefasst */   
  
  double x,y;  /* x und y sind Speicherplaetze fuer Gleitkommazahlen */ 

  x=2.0;       /* 1.2 wird an Speicherstelle x gespeichert */  
  x=pow(x,2.0);
  printf("x= %7.2f \n",x);
  
  return 0;                       /* Rueckgabe einer 0 an die aufrufende 
				     Funktion (Betriebssytem) */ 
}