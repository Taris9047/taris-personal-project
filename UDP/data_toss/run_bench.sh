#!/bin/bash
# set -x

#
# Run mode can be one of pgi, release, and llvm. Yeap, just check up Makefile
#
RUN_MODE=$1

#
# Number of threads to benchmark. (Edit this array for your convenience)
#
THREAD_ARY=(1 2 3 4 5 6 7 8 16 24 32 48 72 96 128 192)

# Clean up screen
# reset

# Let's check up if ./data_toss exists.
DATA_TOSS=$(command -v ./data_toss)
if [[ -z $DATA_TOSS ]]; then
  echo "Can't find ./data_toss ..."
  echo ""
  if [[ -z $RUN_MODE ]]; then
    echo "Run Mode has not given, assuming release"
    echo ""
    RUN_MODE="release"
    make $RUN_MODE
  else
    echo "Compiling with given run mode: $RUN_MODE"
    echo ""
    make clean
    make $RUN_MODE
  fi # if [[ -z $RUN_MODE ]]; then
  DATA_TOSS="./data_toss"
else
  if [[ -z $RUN_MODE ]]; then
    echo "$DATA_TOSS found. Using it."
    echo ""
  else
    echo "Recompiling with given run mode: $RUN_MODE"
    echo ""
    make clean
    make $RUN_MODE
  fi # if [[ -z $RUN_MODE ]]; then
fi # if [[ -z $DATA_TOSS ]]; then



# Let's run the benchmark!!
for th in "${THREAD_ARY[@]}"
do
  eval "$DATA_TOSS -q -t $th"
  echo "$th done!!"
  echo ""
done
