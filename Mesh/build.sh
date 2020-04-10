#!/bin/bash

# cpu number?
if [ -f "/proc/cpuinfo" ]; then
	CPUNUM=$(cat /proc/cpuinfo | awk '/^processor/{print $3}' | tail -1)
else
	CPUNUM=2
fi

# Get argument
ARG=$1

# Clean up
source ./clean.sh
make clean && reset && clear

if [[ $ARG == "" || $ARG == "debug" ]] ; then
    make -j $CPUNUM all
elif [[ $ARG == "release" ]] ; then
    # It doesn't work at this moment.
    make -j $CPUNUM release
fi
