/* Datei: beispiel-2.1.c
   Datum: 12.4.2016 */  

/* #include Anweisungen binden Erweiterungen ein (hier I/O) */  

#include <stdio.h>
#include <stdlib.h>


/* Funktion "main" wird bei Start des Programms ausgefuehrt */ 
int main()   /* int definiert den Datentyp der Groesse, 
                die "main" zurueck gibt */ 
{            /* geschweifte Klammern begrenzen Programmbloecke
                hier werden alle Anweisungen der Funktion 
                eingefasst */   
  
  double x,y;  /* x und y sind Speicherplaetze fuer Gleitkommazahlen */ 

  x=1.2;       /* 1.2 wird an Speicherstelle x gespeichert */  

  printf("x= %7.2f \n",x);
  
  return 0;                       /* Rueckgabe einer 0 an die aufrufende 
				     Funktion (Betriebssytem) */ 
}    /* geschweifte Klammer zum Abschluss der Funktion */ 

/* Das Programm kann mit 
   
        gcc beispiel-2.1.c -o beispiel-2.1 

   kompiliert  und dann mit 
    
        ./beispiel-2.1   

   ausgefuehrt werden. 

   Ergebnis: 

      x = 1.20

*/
