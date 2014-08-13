#!/bin/sh

./brownian 50 1 
gnuplot ./plot.gp 
convert -density 300 ./trace.eps ./trace.pdf
open ./trace.pdf