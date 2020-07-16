set terminal pngcairo size 1100,1000 enhanced
set size square

set lmargin screen 0.0
set rmargin screen 0.99
set tmargin screen 0.95
set bmargin screen 0.1

set border linewidth 0

set xlabel "p"
set ylabel "p'"

set xrange [0:200]
set yrange [0:200]

set cbrange [0:2]
set output "plot1.png"
set title "n_y = 10^1"

fn(x) = x/2. +0.5
set pm3d map
splot "plot1" matrix u (fn($1)):(fn($2)):3


set cbrange [0:0.1]
set output "plot2.png"
set title "n_y = 10^2"

fn(x) = x/2. +0.5
set pm3d map
splot "plot2" matrix u (fn($1)):(fn($2)):3

set cbrange [0:0.0000001]
set output "plot3.png"
set title "n_y = 10^3"

fn(x) = x/2. +0.5
set pm3d map
splot "plot3" matrix u (fn($1)):(fn($2)):3

set output "plot4.png"
set title "n_y = 10^4"

fn(x) = x/2. +0.5
set pm3d map
splot "plot4" matrix u (fn($1)):(fn($2)):3

set output "plot5.png"
set title "n_y = 10^5"

fn(x) = x/2. +0.5
set pm3d map
splot "plot5" matrix u (fn($1)):(fn($2)):3
