#!/usr/bin/env gnuplot
set terminal postscript eps enhanced color "Helvetica" 24
#set terminal pdf enhanced fname 'Helvetica' fsize 10
#set terminal aqua
set datafile separator ","

set style line 1 lw 4
set style line 2 lw 4
set style line 3 lw 4
set style line 4 lw 4

set output 'recv_pthread_perf_plot.eps'
set xlabel '# of threads'
set ylabel 'Giga bits per second'
set title 'Data Rate Comparison (POSIX threads)'
set key bottom right

plot 'recv_benchmark.txt' using ($1):($2*1e-9) title "PGI 17.4-0" w l ls 1, \
     'recv_benchmark.txt' using ($1):($3*1e-9) title "GCC 7.2.0" w l ls 2, \
     'recv_benchmark.txt' using ($1):($4*1e-9) title "Clang 5.0.0" w l ls 3
