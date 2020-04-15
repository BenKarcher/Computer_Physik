/* Datei: beispiel-2.2.c    Datum: 12.4.2016 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h> /* mathematische Funktionen */

double f(double x) /* Funktion f gibt double-Wert zurueck  und hat ein Argument "x" mit dem Typ double */
{
  return sin(x); /* es wird der sin des Argument zurueckgeben */
} /* Ende f */

int main()
{
  int i, n = 10; /* i,n sind int Variablen, n wird mit 100 vorbelegt */
  double y[100]; /* Speicher fuer 100 Gleitkommazahlen */

  /* "for" Schleife */
  for (i = 0; i < n; i++)
  /* 1) Startwert fuer i=0  2) ausfuehren solange i<n ist  3) am Ende i um 1 erhoehen (i++)  und wieder zu 2) */
  { /* Block mit Anweisungen */
    y[i] = f(1.5 * i);
  }

  /* while-Schleife */
  i = 0;
  while (i != n) /* solange Ausfuehren wie i!=n */
  {
    y[i] = f(1.5 * i);
    i = i + 1; /* entspricht der i++ Anweisung von oben */
  }

  /* do-while-Schleife      
     Bedingung wird am Ende geprueft <=> Schleife wird mindestens 1x ausgefuehrt */

  i = 0;
  do
  {
    y[i] = f(1.5 * i);
    i = i + 1;         /* entspricht der i++ Anweisung von oben */
  } while (!(i >= n)); /* solange Ausfuehren wie !(i>=n) */
} /* Ende main */
