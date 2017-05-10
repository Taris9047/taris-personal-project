#!/usr/bin/env gnuplot
set terminal jpeg

set datafile separator ","
set output "SpeedResults.jpg"

set xlabel "Sessions"
set ylabel "Mbps"
set title "0MQ send_msg speed test"
show margin

plot "result.txt" using ($1):($4/1000000) title "psDAC 0.1.4"