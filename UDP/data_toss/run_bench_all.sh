#!/bin/bash
# set -x

RUN_MODES=(pgi release llvm)

# Let's run the benchmark!!
for rm in "${RUN_MODES[@]}"
do
  eval "./run_bench.sh $rm >& ${rm}_result.txt"
  echo "$rm done!!"
  echo ""
done
