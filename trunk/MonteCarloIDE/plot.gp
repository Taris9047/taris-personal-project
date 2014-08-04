#!/usr/bin/env gnuplot --persist
set terminal postscript eps enhanced color "Helvetica" 24

IN_FILENAME = "trace.txt"
OUT_FILENAME = "trace.eps"
XRANGE = 1000
YRANGE = 1000

set output OUT_FILENAME
set xrange [-XRANGE:XRANGE]
set yrange [-YRANGE:YRANGE]
show margin

set samples XRANGE+XRANGE+1
set isosamples YRANGE+YRANGE+1

plot IN_FILENAME using 1:2:3:4 w vec title "Brownian Movement"
