import PyGnuplot as gp
import numpy as np
X = np.arange(42)
Y = np.zeros(42)
# Load file
file1 = open('plots/plot00', 'r')
Lines = file1.readlines()

i=0
for line in Lines:
    nums = line.split(' ')
    for i in range(len(nums)):
        Y[i] = nums[i]
gp.s([X, Y])
gp.c('set terminal pngcairo size 1100,1000 enhanced')
gp.c('set output "plot1.png"')
gp.c('plot "tmp.dat" u 1:2 w lp')
