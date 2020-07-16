/* beispiel-4.4-numerov.c   Datum:  06.05.2020 */
/* dieser Code implementiert ein vereinfachtes Numerov Problem, 
   Erweiterung notwendig bei "Korrektur notwendig fuer g!=0 !!!!" */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Routine fuer Nullstellensuche mit dem Sekantenverfahren (siehe beispiel-3.3.c) */

double secant(double x1, double x2, double (*func)(double), int *schritt)
/* x1,x2     Startwerte 
   func      ist die "Referenz" auf eine Funktion mit einem double Parameter, 
             die double zurueckgibt (Referenz = Adresse der Funktion)  
   schritt   ist auch Referenz: Veraenderungen an der Variable wirken sich auf 
                                das aufrufende Programm aus !!! */
{
  const double tol = 1e-12; /* geforderte Genauigkeit als Konstante */
  double xn;                /* neuer Schaetzwert */

  *schritt = 0; /* noch kein Schritt */

  do
  { /* naechster Schaetzwert x1,x2 -> xn*/
    xn = x2 - func(x2) * (x2 - x1) / (func(x2) - func(x1));
    x1 = x2; /* bereite den naechsten Schritt vor:  x2 -> x1 */
    x2 = xn; /*                                     xn -> x2 */

    (*schritt)++;                /* Schritte=Schritte+1 */
  } while (fabs(x2 - x1) > tol); /* solange Genauigkeitsziel nicht erreicht */

  return xn; /* Gebe Nullstelle zurueck */
}

/* Code fuer die Loesung der Poisson-Gleichung mit gaussfoermiger Ladungsverteilung */

/* wegen Nullstellensuche definiere globale Felder, die in 
   Funktion zur Nullstellensuche genutzt werden koennen */

double r0bound;                                  /* definiert radius der homogenen Ladungsverteilung */
double *r_array, *g_array, *s_array, *y_loesung; /* Zeiger auf Felder fuer Numerov und Loesung */
double schrittweite_h;                           /* benutzte Schrittweite */
int num_r;                                       /* Anzahl der Stuetzstellen */

double gfunc(double r)
/* zu loesende Gleichung: y''(r)+g(r)*y(r)=s(r) 
   hier Definition der Funktion g(r)   */
{
  return 0.0;
}

double sfunc(double r)
// zu loesende Gleichung: y''(r)+g(r)*y(r)=s(r)
// hier Definition der Funktion s(r)
{

  return -4.0 * r * M_PI * exp(-(r / r0bound) * (r / r0bound)) / (r0bound * r0bound * r0bound * sqrt(M_PI * M_PI * M_PI));
}

/* fuer konstante Verteilung, vorsicht Unstetigkeit fuehrt zu Ungenauigkeit !!!!
double sfunc(double r)
// zu loesende Gleichung: y''(r)+g(r)*y(r)=s(r) 
//   hier Definition der Funktion s(r)
// Beispiel einer unstetigen Funktion    
  {
    if(r>r0bound)
      {
	return 0.0; 
      }
    else
      {
	return -4.0*r*1.0/(4.0*M_PI/3.0*r0bound*r0bound*r0bound); 
      }  
      } */

double init_numerov(double a, double b, int n, double *r, double *g, double *s)
/* Funktionen bereitet Anwendung des Numerov Verfahrens vor 
   a,b,n sind Intervallgrenzen und Anzahl der Stuetzstellen 
   bei Verlassen der Routine beinhalten die Felder r,g und s 
   Stuetzstellen, Funktion g an den Stuetzstellen und s an den Stuetzstellen
   Rueckgabewert ist die Schrittweite h */
{
  double h = (b - a) / (n - 1);  /* bestimme Intervalllaenge */
  for (int i = 0; i < n; i++) /* belege die Felder mit den Werten fuer r,g und s*/
  {
    r[i] = a + i * h;
    g[i] = gfunc(r[i]);
    s[i] = sfunc(r[i]);
  }

  return h; /* Rueckgabewert ist Schrittweite h */
}

void numerovup(double *r, double *g, double *s, double h, int n, int steps, double y0, double y1, double *y)
/* Funktion nutzt das Numerov Verfahren um fuer gegebene Stuetzstellen r
   und Funktionen g und s die Loesung y zu finden. 
   r, g und s sollten mit init_numerov vorbereitet werden
   h ist die Schrittweite (auch aus init_numerov)
   n ist die Anzahl der Stuetzstellen 
   steps legt fest, wieviele Numerovschritte ausgefuehrt werden
   y0 und y1 sind die Startwerte y[0] und y[1], die Routine legt dann y[2] .. y[steps+1] fest 
   y ist ein Feld der Laenge  steps+2, das die Loesung bei Rueckkehr enthaelt
*/
{
  int i;
  double fakt_u_np1, fakt_u_n, fakt_u_nm1; /* Variablen fuer Numerov Faktoren */
  double fakt_s;

  y[0] = y0; /* belege erste Funktionswerte mit den Startwerten  */
  y[1] = y1;

  for (i = 1; i < steps + 1; i++) /* betrachte in den Schritten y(i-1) und y(i) um y(i+1) zu berechnen */
  {
    fakt_u_np1 = 1.0;                                            /* Faktor bei y(i+1),  Korrektur notwendig fuer g!=0 !!!! */
    fakt_u_n = 1.0;                                              /* Faktor bei y(i),    Korrektur notwendig fuer g!=0 !!!! */
    fakt_u_nm1 = 1.0;                                            /* Faktor bei y(i-1)   Korrektur notwendig fuer g!=0 !!!! */
    fakt_s = h * h / 12.0 * (s[i + 1] + 10.0 * s[i] + s[i - 1]); /* Faktor mit s */

    y[i + 1] = (fakt_s + 2.0 * fakt_u_n * y[i] - fakt_u_nm1 * y[i - 1]) / fakt_u_np1; /* Vorwaertsiteration */
  }
}

void numerovdown(double *r, double *g, double *s, double h, int n, int steps, double yn, double ynm1, double *y)
/* Funktion nutzt das Numerov Verfahren um fuer gegebene Stuetzstellen r
   und Funktionen g und s die Loesung y zu finden. 
   r, g und s sollten mit init_numerov vorbereitet werden
   h ist die Schrittweite (auch aus init_numerov)
   n ist die Anzahl der Stuetzstellen 
   steps legt fest, wieviele Numerovschritte ausgefuehrt werden
   yn und ynm1 sind die Startwerte y[n-1] und y[n-2], die Routine legt dann y[n-3] .. y[n-steps-2] fest 
   y ist ein Feld der Länge n, das die Loesungen
*/
{
  int i;
  double fakt_u_np1, fakt_u_n, fakt_u_nm1; /* Variablen fuer Numerov Faktoren */
  double fakt_s;

  y[n - 1] = yn; /* belege erste Funktionswerte mit den Startwerten  */
  y[n - 2] = ynm1;

  for (i = n - 2; i > n - steps - 2; i--) /* betrachte in den Schritten y(i-1) und y(i) um y(i+1) zu berechnen */
  {
    fakt_u_np1 = 1.0;                                            /* Faktor bei y(i+1),  Korrektur notwendig fuer g!=0 !!!! */
    fakt_u_n = 1.0;                                              /* Faktor bei y(i),    Korrektur notwendig fuer g!=0 !!!! */
    fakt_u_nm1 = 1.0;                                            /* Faktor bei y(i-1)   Korrektur notwendig fuer g!=0 !!!! */
    fakt_s = h * h / 12.0 * (s[i + 1] + 10.0 * s[i] + s[i - 1]); /* Faktor mit s */

    y[i - 1] = (fakt_s + 2.0 * fakt_u_n * y[i] - fakt_u_np1 * y[i + 1]) / fakt_u_nm1; /* Rueckwaertsiteration */
  }
}

double f_randbed(double para)
/* Nullstelle dieser Funktion signalisiert, dass die Loesung 
   konsistent mit der Randbedinung ist
   para ist der freie Parameter der Loesung 
   hier: para ist der Konstantewert der Loesung bei großen r 
   Die Funktion nutzt globale Variabeln, um die Felder 
   r, g, s, h und n zu erhalten   
   und feld y_loesung fuer die Loesung */
{
  numerovdown(r_array, g_array, s_array, schrittweite_h, num_r, num_r - 2, para, para, y_loesung);
  return y_loesung[0]; /* zweite Randbedingung ist y[0] = 0 */
}

int main()
{
  double rmax, para, zero; /* maximales r und konsistente Randbedingung und Null aus Auswertung der Randbedingungsfunktion */
  int i, num_schritt;      /* Anzahl der Sekantenverfahren Schritte */
  double rho;              /* fuer Ausgabe der Ladungsverteilung */

  printf("# Geben Sie rmax,r0 und die Anzahl der Stuetzstellen ein:");
  scanf(" %le %le %d", &rmax, &r0bound, &num_r);

  r_array = malloc(num_r * sizeof(double)); /* alloziere Speicher fuer r, g und s */
  g_array = malloc(num_r * sizeof(double));
  s_array = malloc(num_r * sizeof(double));
  y_loesung = malloc(num_r * sizeof(double));

  schrittweite_h = init_numerov(0.0, rmax, num_r, r_array, g_array, s_array); /* belege r,g und s mit Werten*/

  /* suche mit dem Schiessverfahren und mit Numerov die Loesung, die den Randbedingungen genuegt. */

  para = secant(-1.0, 1.0, &f_randbed, &num_schritt);

  zero = f_randbed(para);

  /* Ausgabe der Loesung phi/r und Vergleich mit para/r */

  printf("# %15s %15s %15s \n", "r", "Phi", "para/r");

  for (i = 1; i < num_r; i++)
  {
    printf(" %15.6e %15.6e %15.6e \n", r_array[i], y_loesung[i] / r_array[i], para / r_array[i]);
  }

  printf("#Parameter, Null und Anzahl der secant Schritte: %15.6e %15.6e %d \n", para, zero, num_schritt);

  free(r_array);
  free(g_array);
  free(s_array);
  free(y_loesung);

  return 0;
}
