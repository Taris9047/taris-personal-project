#!/usr/bin/env gnuplot --persist
set terminal postscript eps enhanced color "Helvetica" 24

IN_FILENAME = "trace.csv"
OUT_FILENAME = "trace.eps"
XRANGE = 1000
YRANGE = 1000

set output OUT_FILENAME
set xrange [-XRANGE:XRANGE]
set yrange [-YRANGE:YRANGE]
set xlabel "X Dimension"
set ylabel "Y Dimension"
set y2label "Time (sec.)"
show margin
set key top left

set samples XRANGE+XRANGE+1
set isosamples YRANGE+YRANGE+1

plot IN_FILENAME using ($1):($2):($3):($4):($7) w vectors head empty size 0.030,15,135 lc palette notitle
