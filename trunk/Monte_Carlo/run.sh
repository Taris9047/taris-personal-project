#!/bin/sh

make
./brownian $1 $2 $3
gnuplot ./plot.gp 
convert -density 300 ./trace.eps ./trace.pdf
open ./trace.pdf
