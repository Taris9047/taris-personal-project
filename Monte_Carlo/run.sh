#!/bin/sh

make
./brownian $1 $2 
gnuplot ./plot.gp 
convert -density 300 ./trace.eps ./trace.pdf
open ./trace.pdf
