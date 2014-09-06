#!/bin/bash

echo "Trying to compile... if anything changed in CPP code."
if [ $OSTYPE == "cygwin" ]; then
	make cyg -j 6
else
	make -j 6
fi

echo "Running the brownian movement simulator with arguments."
if [ -z $1 ]; then
	./brownian 20 1 1
else
	./brownian $1 1 $2
fi

echo "Plotting the log with gnuplot."
if [ -z `which gnuplot` ]; then
	echo "Gnuplot can't be found!!"
	break
else
	`which gnuplot` ./plot.gp
fi

echo "Converting the .eps to .pdf"
if [ -z convert ]; then
	echo "Please install Imagemagick. This script uses convert from that package!"
	break
else
	convert -density 300 ./trace.eps ./trace.pdf
fi

echo "Opening the .pdf file"
if [ $OSTYPE == "linux-gnu" ]; then
    evince ./trace.pdf
elif [ `uname` == "Darwin" ]; then
    open ./trace.pdf
elif [ $OSTYPE == "cygwin" ]; then
    xpdf ./trace.pdf
else
    echo "Open ./trace.pdf to watch the show!!"
fi

