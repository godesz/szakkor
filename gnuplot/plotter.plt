# http://gnuplot.sourceforge.net/demo_5.2/

set terminal windows
set grid

set xrange [0:1000]
set yrange [-50:50]

set title "MFG logging"
set xlabel "time(s)"
set ylabel "data(m)"

filename = "datafile.dat"
plot filename using ($0/10):1 with lines lt 2 title "HR"