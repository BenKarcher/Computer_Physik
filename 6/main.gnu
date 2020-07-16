set terminal pngcairo size 1500,300 enhanced

set xlabel 't'

set ylabel 'Amplitude'
do for [t = -1:3]{
    a = 2**t
    Title = sprintf('a = b = %.1f',a)
    set title Title
    infile  = sprintf("plots/echo/%.1f.plot", a);
    outfile = sprintf("plots/echo/%.1f.png", a);
    set output outfile
    plot infile using 1:2
}

set ylabel '|s⊙e|^2'
do for [t = -1:3]{
    a = 2**t
    Title = sprintf('a = b = %.1f',a)
    set title Title
    infile  = sprintf("plots/korrelation/%.1f.plot", a);
    outfile = sprintf("plots/korrelation/%.1f.png", a);
    set output outfile
    plot infile using 1:2
}

do for [t = -1:3]{
    a = 2**t
    Title = sprintf('a = b = %.1f',a)
    set title Title
    infile  = sprintf("plots/korrelation/%.1f.plot", a);
    outfile = sprintf("plots/korrelation/%.1f.png", a);
    set output outfile
    plot infile using 1:2
}

set output "plots/analytisch/analytisch.png"
set title "analytisch"
plot "plots/analytisch/analytisch.plot" using 1:2

set output "plots/analytisch/numerisch.png"
set title "numerisch"
plot "plots/analytisch/numerisch.plot" using 1:2

do for [t = -1:3]{
    a = 2**t
    Title = sprintf('a = b = %f',a)
    set title Title
    infile  = sprintf("plots/zwitscher/%.1f.plot", a);
    outfile = sprintf("plots/zwitscher/%.1f.png", a);
    set output outfile
    plot infile using 1:2
}