#!/bin/sh

./brownian && gnuplot ./plot.gp 
convert -density 300 ./trace.eps ./trace.pdf
open ./trace.pdf