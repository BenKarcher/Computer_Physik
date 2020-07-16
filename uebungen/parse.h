#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED
struct expression
{
    double (*func)(double, double);
    struct expression *a, *b;
    double result;
};
struct expression *expression(double (*func)(double, double),struct expression *a, struct expression *b, double result);
struct expression *constant(double k);
struct expression *copy(struct expression *f);


double eval(struct expression *equ, double x);
struct expression *parse_recursive(char *start, char *end);
struct expression *parse(char *str);
void deepfree(struct expression *expression);
struct expression *derive(struct expression *f);

int starts_with(char *start, char *cmp);

double _add_(double a, double b);
double _subtract_(double a, double b);
double _multiply_(double a, double b);
double _divide_(double a, double b);
double _negative_(double a, double b);
double _pow_(double a, double b);
double _sin_(double a, double b);
double _cos_(double a, double b);
double _sqrt_(double a, double b);
double _exp_(double a, double b);
double _ln_(double a, double b);
double _const_(double a, double b);
double _x_(double a, double b);
double _factorial_(double a, double b);
double _abs_(double a, double b);

#endif