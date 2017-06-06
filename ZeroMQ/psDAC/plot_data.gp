#!/usr/bin/env gnuplot
set terminal jpeg

set datafile separator ","
set output "SpeedResults.jpg"

set xlabel "Sessions"
set ylabel "Mbps"
set title "0MQ send_msg speed test"
show margin

major_ver=system('cat ./psDAC.h | grep "\#define VERSION_MAJOR" | cut -d" " -f 3')
minor_ver=system('cat ./psDAC.h | grep "\#define VERSION_MINOR" | cut -d" " -f 3')
patch_ver=system('cat ./psDAC.h | grep "\#define VERSION_PATCH" | cut -d" " -f 3')

current_graph_label=sprintf("psDAC %s.%s.%s", major_ver, minor_ver, patch_ver)

plot "result.txt" using ($1):($4/1000000) title current_graph_label