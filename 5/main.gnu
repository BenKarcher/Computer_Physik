set terminal pngcairo size 1100,1000 enhanced

set xlabel "x"
set ylabel "u"

set xrange [-5:30]
set yrange [-10:3]
tmin = -1.
tmax = 1.
numFrames = 100

do for [t = 0:(numFrames-1)]{
    time = tmin+(tmax-tmin)*t/(numFrames-1)
    Title = sprintf('t = %fs',time)
    set title Title
    infile  = sprintf("plots/plotExact/time%02d", t);
    outfile = sprintf("plots/plotExact/time%02d.png", t);
    set output outfile
    plot infile using 1:2 w l
}

numFrames = 250
set xrange [-5:45]
set yrange [-20:3]
do for [t = 0:(39)]{
    time = tmin+(tmax-tmin)*t/(39)
    Title = sprintf('t = %fs',time)
    set title Title
    infile  = sprintf("plots/plot%02d/time%02d", 0, t);
    outfile = sprintf("plots/plot%02d/time%02d.png", 0, t);
    set output outfile
    plot infile using 1:2 w l title 'u(t,x)'
}
do for [i = 1:3]{
    do for [t = 0:(numFrames-1)]{
        time = tmin+(tmax-tmin)*t/(numFrames-1)
        Title = sprintf('t = %fs',time)
        set title Title
        infile  = sprintf("plots/plot%02d/time%02d", i, t);
        outfile = sprintf("plots/plot%02d/time%02d.png", i, t);
        set output outfile
        plot infile using 1:2 w l title 'u(t,x)'
    }
}

# N=1
unset xrange
unset yrange
set output "plots/vergleichN1.png"
plot "plots/plotN100/time39" using 1:2 w l title 'h = 0.4',\
     "plots/plotN101/time249" using 1:2 w l title 'h = 0.2',\
     "plots/plotN102/time249" using 1:2 w l title 'h = 0.1',\
     "plots/plotN103/time249" using 1:2 w l title 'h = 0.05',\
     "plots/plotExactN1/time199" using 1:2 w l title 'exakt'

# N=2
unset xrange
unset yrange
set output "plots/vergleichN2.png"
plot "plots/plot01/time249" using 1:2 w l title 'h = 0.2',\
     "plots/plot02/time249" using 1:2 w l title 'h = 0.1',\
     "plots/plot03/time249" using 1:2 w l title 'h = 0.05',\
     "plots/plotExact/time99" using 1:2 w l title 'exakt'

# N variieren
set xrange [-5:15]
set yrange [-8:3]
do for [i = 0:9]{
    do for [t = 0:(numFrames-1)]{
        time = tmin+(tmax-tmin)*t/(numFrames-1)
        Title = sprintf('t = %fs',time)
        set title Title
        infile  = sprintf("plots/plotN1.%1d/time%02d", i, t);
        outfile = sprintf("plots/plotN1.%1d/time%02d.png", i, t);
        set output outfile
        plot infile using 1:2 w l title 'u(t,x)'
    }
}

# Alle N in einem
system "mkdir plots/plotNOut"
numFrames = 250
set xrange [-5:15]
set yrange [-8:3]
do for [t = 0:(numFrames-1)]{
    time = tmin+(tmax-tmin)*t/(numFrames-1)
    Title = sprintf('t = %fs',time)
    set title Title
    outfile = sprintf("plots/plotNOut/time%02d.png", t);
    set output outfile
    plot for [N=0:9] sprintf("plots/plotN1.%d/time%02d", N, t) using 1:2 w l title 'N1.'.N
}

#N = 3
set xrange [-5:45]
set yrange [-20:3]
numFrames = 250
do for [t = 0:(numFrames-1)]{
    time = tmin+(tmax-tmin)*t/(numFrames-1)
    Title = sprintf('t = %fs',time)
    set title Title
    infile  = sprintf("plots/plotN3/time%02d", t);
    outfile = sprintf("plots/plotN3/time%02d.png", t);
    set output outfile
    plot infile using 1:2 w l title 'u(t,x)'
}

#HeatMap
set border linewidth 0

set xlabel "d"
set ylabel "h"

set xrange [0.00003:0.00025]
set yrange [0.05:0.4]

set cbrange [0:0.2]
set title "N=1"

fd(x) = ((x+0.5)/88)*(0.00025-0.00003)+0.00003
fh(x) = ((x+0.5)/117)*(0.4-0.05)+0.05

f(x) = (2.6*x)**(1./3.)
set output "plots/heatMapN1.png"
set multiplot

set pm3d map
splot "plots/heatMapN1" matrix u (fd($1)):(fh($2)):3 with pm3d
unset pm3d
set size 0.83,0.83
set origin 0.088,0.11

unset xlabel
unset ylabel
unset key
unset xtics
unset ytics
unset border
unset title

plot f(x) lw 2 lt rgb "cyan"
unset multiplot

reset

set output "plots/heatMapN2.png"
set multiplot
set border linewidth 0

set xlabel "d"
set ylabel "h"
set key
set xtics
set ytics

set xrange [0.00003:0.00025]
set yrange [0.05:0.4]

set cbrange [0:0.2]

set title "N=2"

set pm3d map
splot "plots/heatMapN2" matrix u (fd($1)):(fh($2)):3 with pm3d
unset pm3d
set size 0.83,0.83
set origin 0.088,0.11

unset xlabel
unset ylabel
unset key
unset xtics
unset ytics
unset border
unset title

plot f(x) lw 2 lt rgb "cyan"
unset multiplot
