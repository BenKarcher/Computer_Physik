#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(void){
    struct expression *f = parse("exp(3*x^2+4*x)");
    printf("f(0)=%lf\n",eval(f,0));
    struct expression *g = derive(f);
    printf("f'(0)=%lf\n",eval(g,0));
}