#!/bin/bash

echo "Trying to compile... if anything changed in CPP code."
if [ $OSTYPE == "cygwin" ]; then
	make cyg -j 6
else
	make -j 6
fi

echo "Running the brownian movement simulator with arguments."
./brownian $1 $2 $3

echo "Plotting the log with gnuplot."
gnuplot ./plot.gp 

echo "Converting the .eps to .pdf"
convert -density 300 ./trace.eps ./trace.pdf

echo "Opening the .pdf file"
if [ $OSTYPE == "linux-gnu" ]; then
    evince ./trace.pdf
elif [ $OSTYPE == "darwin" ]; then
    open ./trace.pdf
elif [ $OSTYPE == "cygwin" ]; then
    xpdf ./trace.pdf
else
    echo "Open ./trace.pdf to watch the show!!"
fi

