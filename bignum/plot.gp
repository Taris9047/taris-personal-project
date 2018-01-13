#!/usr/bin/env gnuplot --persist
set terminal postscript eps enhanced color "Helvetica" 24

set style line 1 lw 8
set style line 2 lw 8

IN_FILE = 'PBNvsBNI.txt'
OUT_FILE = 'factorial_comparison.eps'

set output OUT_FILE
set xlabel 'Integer for factorial (#)'
set ylabel 'Time (seconds)'
set xrange [:50]
set format y '%.2E'
set xrange [1:]
set title 'Factorial performance comparison for PBY.py'
set key top left

plot IN_FILE using ($1):($2) title 'PBN.py' w l ls 1, \
    IN_FILE using ($1):($3) title 'BignumInt.cpp' w l ls 2
