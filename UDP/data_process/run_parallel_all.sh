#!/bin/bash
# set -x

RUN_MODES=(pgi release llvm)

THREADS=(1 2 3 4 5 6 7 8 16 24 32 48 72 96 128 192)

# Let's run the benchmark!!
for rm in "${RUN_MODES[@]}"
do
  make clean
  make $rm
  for th in "${THREADS[@]}"
  do
    eval "./run_parallel.sh $th"
    echo "[$rm] Threads $th done"
  done
  mv ./*_results.txt ./results
  echo "$rm done!!"
  echo ""
done
