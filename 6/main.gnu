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
#set xrange [45:55]
# set yrange [0:6.25]
# set y2range [0:1.4e-7]

# set output "plots/analKorrelation.png"
# set title "analytisch"
# plot "plots/analKorrelation.cppplot" using 1:2 w l title "analytisch", "plots/korrelation/korrelation0.cppplot" using 1:2 w l axes x1y2 title "numerisch"

# unset yrange

# set output "plots/korrelation/korrelation0.png"
# set title "a=b=0"
# plot "plots/korrelation/korrelation0.cppplot" using 1:2 w l

# set output "plots/korrelation/korrelation0.5.png"
# set title "a=b=0.5"
# plot "plots/korrelation/korrelation0.5.cppplot" using 1:2 w l

# set output "plots/korrelation/korrelation1.png"
# set title "a=b=1"
# plot "plots/korrelation/korrelation1.cppplot" using 1:2 w l

# set output "plots/korrelation/korrelation2.png"
# set title "a=b=2"
# plot "plots/korrelation/korrelation2.cppplot" using 1:2 w l

# set output "plots/korrelation/korrelation4.png"
# set title "a=b=4"
# plot "plots/korrelation/korrelation4.cppplot" using 1:2 w l

# set output "plots/korrelation/korrelation8.png"
# set title "a=b=8"
# plot "plots/korrelation/korrelation8.cppplot" using 1:2 w l

# set output "plots/korrelation/korrelation16.png"
# set title "a=b=16"
# plot "plots/korrelation/korrelation16.cppplot" using 1:2 w l


# set output "plots/korrelation/korrelationB.png"
# set title "a=b=4, zwitschern"
# plot "plots/korrelation/korrelationB.cppplot" using 1:2 w l

# set output "plots/korrelation/korrelationB0.5.png"
# set title "a=b=0.5, zwitschern"
# plot "plots/korrelation/korrelationB0.5.cppplot" using 1:2 w l

# set output "plots/korrelation/korrelationB1.png"
# set title "a=b=1, zwitschern"
# plot "plots/korrelation/korrelationB1.cppplot" using 1:2 w l

# set output "plots/korrelation/korrelationB2.png"
# set title "a=b=2, zwitschern"
# plot "plots/korrelation/korrelationB2.cppplot" using 1:2 w l

# set output "plots/korrelation/korrelationB8.png"
# set title "a=b=8, zwitschern"
# plot "plots/korrelation/korrelationB8.cppplot" using 1:2 w l

# set output "plots/korrelation/korrelationB16.png"
# set title "a=b=16, zwitschern"
# plot "plots/korrelation/korrelationB16.cppplot" using 1:2 w l
