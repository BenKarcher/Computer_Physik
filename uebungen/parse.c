#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double _add_(double a, double b) { return a + b; };
double _subtract_(double a, double b) { return a - b; };
double _multiply_(double a, double b) { return a * b; };
double _divide_(double a, double b) { return a / b; };
double _negative_(double a, double b) { return -a; };
double _pow_(double a, double b) { return pow(a, b); };
double _sin_(double a, double b) { return sin(a); };
double _cos_(double a, double b) { return cos(a); };
double _sqrt_(double a, double b) { return sqrt(a); };
double _exp_(double a, double b) { return exp(a); };
double _ln_(double a, double b) { return log(a); };
double _const_(double a, double b) { return a; };
double _x_(double a, double b) { return b; };
double _factorial_(double a, double b) { return tgamma(a + 1); };
double _abs_(double a, double b) { return fabs(a); };

struct expression *expression(double (*func)(double, double), struct expression *a, struct expression *b, double result)
{
    struct expression *res = (struct expression *)malloc(sizeof(struct expression));
    res->a = a;
    res->b = b;
    res->result = result;
    res->func = func;
    return res;
}
struct expression *constant(double k)
{
    struct expression *res = (struct expression *)malloc(sizeof(struct expression));
    res->a = NULL;
    res->b = NULL;
    res->result = k;
    res->func = _const_;
    return res;
}
struct expression *copy(struct expression *f)
{
    return expression(
        f->func,
        (f->a) ? copy(f->a) : NULL,
        (f->b) ? copy(f->b) : NULL,
        f->result);
}

struct expression *parse(char *str)
{
    return parse_recursive(str, str + strlen(str) - 1);
}

int starts_with(char *start, char *cmp)
{
    for (size_t i = 0; i < strlen(cmp); i++)
    {
        if (start[i] == '\0' || start[i] != cmp[i])
        {
            return 0;
        }
    }
    return 1;
}

struct expression *parse_recursive(char *start, char *end)
{
    int depth = 0;
    int paranthasis_only = 1;
    for (char *ptr = end; ptr > start; ptr--)
    {
        if (*ptr == ')')
        {
            depth++;
        }
        else if (*ptr == '(')
        {
            depth--;
            if (depth < 0)
            {
                printf("panik");
            }
        }
        else if (depth == 0)
        {
            paranthasis_only = 0;
            if (*ptr == '+' || *ptr == '-')
            {
                return expression(
                    (*ptr == '+') ? _add_ : _subtract_,
                    parse_recursive(start, ptr - 1),
                    parse_recursive(ptr + 1, end),
                    0);
            }
        }
    }
    if (*start == '-')
    {
        return expression(
            _negative_,
            parse_recursive(start + 1, end),
            NULL,
            0);
    }
    if (paranthasis_only && *start == '(' && *end == ')')
    {
        return parse_recursive(start + 1, end - 1);
    }
    depth = 0;
    for (char *ptr = end; ptr >= start; ptr--)
    {
        if (*ptr == ')')
        {
            depth++;
        }
        else if (*ptr == '(')
        {
            depth--;
        }
        else if (depth == 0 && (*ptr == '*' || *ptr == '/'))
        {
            return expression(
                (*ptr == '*') ? _multiply_ : _divide_,
                parse_recursive(start, ptr - 1),
                parse_recursive(ptr + 1, end),
                0);
        }
    }
    depth = 0;
    for (char *ptr = end; ptr >= start; ptr--)
    {
        if (*ptr == ')')
        {
            depth++;
        }
        else if (*ptr == '(')
        {
            depth--;
        }
        else if (depth == 0 && *ptr == '^')
        {
            return expression(
                _pow_,
                parse_recursive(start, ptr - 1),
                parse_recursive(ptr + 1, end),
                0);
        }
    }
    if (*end == '!')
    {
        return expression(
            _factorial_,
            parse_recursive(start, end - 1),
            NULL,
            0);
    }
    if (starts_with(start, "sqrt"))
    {
        return expression(_sqrt_, parse_recursive(start + 4, end), NULL, 0);
    }
    else if (starts_with(start, "sin"))
    {
        return expression(_sin_, parse_recursive(start + 3, end), NULL, 0);
    }
    else if (starts_with(start, "cos"))
    {
        return expression(_cos_, parse_recursive(start + 3, end), NULL, 0);
    }
    else if (starts_with(start, "ln"))
    {
        return expression(_ln_, parse_recursive(start + 2, end), NULL, 0);
    }
    else if (starts_with(start, "exp"))
    {
        return expression(_exp_, parse_recursive(start + 3, end), NULL, 0);
    }
    else if (starts_with(start, "abs"))
    {
        return expression(_abs_, parse_recursive(start + 3, end), NULL, 0);
    }

    if (start == end && *start == 'x')
    {
        return expression(_x_, NULL, NULL, 0);
    }

    int length = (end - start) + 2;
    char dest[length];
    strncpy(dest, start, length);
    dest[length] = '\0';
    return constant(atof(dest));
}

double eval(struct expression *equ, double x)
{
    double a = (equ->a) ? eval(equ->a, x) : equ->result;
    double b = (equ->b) ? eval(equ->b, x) : x;
    return equ->result = equ->func(a, b);
}

void deepfree(struct expression *equ)
{
    if (equ->a)
    {
        deepfree(equ->a);
    }
    if (equ->b)
    {
        deepfree(equ->b);
    }
    free(equ);
}

struct expression *derive(struct expression *f)
{
    struct expression *a = f->a;
    struct expression *b = f->b;
    struct expression *result = (struct expression *)malloc(sizeof(struct expression));
    result->result = 0;
    if (f->func == _const_)
    {
        return constant(0);
    }
    else if (f->func == _x_)
    {
        return constant(1);
    }
    else if (f->func == _add_ || f->func == _subtract_)
    {
        return expression(f->func, derive(a), derive(b), 0);
    }
    else if (f->func == _multiply_)
    {
        return expression(
            _add_,
            expression(_multiply_, copy(a), derive(b), 0),
            expression(_multiply_, derive(a), copy(b), 0),
            0);
    }
    else if (f->func == _divide_)
    {
        return expression(
            _divide_,
            expression(
                _subtract_,
                expression(_multiply_, derive(a), copy(b), 0),
                expression(_multiply_, copy(a), derive(b), 0),
                0),
            expression(_pow_, copy(b), constant(2), 0),
            0);
    }
    else if (f->func == _negative_)
    {
        return expression(_negative_, derive(a), NULL, 0);
    }
    else if (f->func == _pow_)
    {
        if (b->func == _const_)
        {
            return expression(
                _multiply_,
                expression(
                    _pow_,
                    copy(a),
                    constant((b->result) - 1),
                    0),
                expression(
                    _multiply_,
                    copy(b),
                    derive(a),
                    0),
                0);
        }
        else
        {
            return expression(
                _multiply_,
                expression(
                    _pow_,
                    copy(a),
                    expression(_subtract_, copy(b), constant(1), 0),
                    0),
                expression(
                    _add_,
                    expression(
                        _multiply_,
                        expression(_multiply_, copy(a), derive(b), 0),
                        expression(_ln_, expression(_abs_, copy(a), NULL, 0), NULL, 0),
                        0),
                    expression(
                        _multiply_,
                        copy(b),
                        derive(a),
                        0),
                    0),
                0);
        }
    }
    else if (f->func == _sin_)
    {
        return expression(
            _multiply_,
            derive(a),
            expression(_cos_, copy(a), NULL, 0),
            0);
    }
    else if (f->func == _cos_)
    {
        return expression(
            _multiply_,
            derive(a),
            expression(_negative_, expression(_sin_, copy(a), NULL, 0), NULL, 0),
            0);
    }
    else if (f->func == _sqrt_)
    {
        return expression(
            _multiply_,
            derive(a),
            expression(_pow_, copy(a), constant(-0.5), 0),
            0);
    }
    else if (f->func == _exp_)
    {
        return expression(_multiply_, derive(a), copy(f), 0);
    }
    else if (f->func == _ln_)
    {
        return expression(_divide_, derive(a), copy(a), 0);
    }
    return constant(0);
}