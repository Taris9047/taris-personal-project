#!/bin/sh -e

# Run mode can be one of pgi, release, and llvm. Yeap, just check up Makefile

RUN_MODE=$1
if [[ -z $SIM_MODE ]]; then
  RUN_MODE="release"
fi

# Clean up screen
reset

DATA_TOSS=$(command -v ./data_toss)
if [[ -z $DATA_TOSS ]]; then
  make $RUN_MODE
else
  make clean
  make $RUN_MODE
fi

THREAD_ARY=(1 2 3 4 5 6 7 8 16 24 32 48 72 96 128 192)

for t in "${THREAD_ARY[@]}"
do
  $DATA_TOSS -q -t "$t"
  echo "$t done!!"
  echo ""
done
