#!/bin/sh -e

# Checking if GNU parallel exists
command -v parallel > /dev/null 2>&1 || { echo >&2 "We need GNU Parallel installed!! Aborting!!"; exit 1; }
PARALLEL_CMD=$(command -v parallel)
echo "GNU Parallel found at $PARALLEL_CMD"

# Checking if ./data_process exists
command -v $PWD/data_process > /dev/null 2>&1 || { echo >&2 "We couldn't find data_process executable!! Trying to compile!!"; make release; }
DATA_TOSS_CMD=$(command -v $PWD/data_process)
if [[ -z $DATA_TOSS_CMD ]]; then
  make release
else
  echo "data_process found at $DATA_TOSS_CMD"
fi

echo ""


# Some parameters
if [[ -z $1 ]]; then
  THREADS=3
else
  THREADS=$1
fi

ITER_CNT=16384

# Ok, let's run it with parallel!!
ADDRS=("127.0.0.1:9930" "127.0.0.1:9931" "127.0.0.1:9932" "127.0.0.1:9933" "127.0.0.1:9934")
# DATA_TOSS_OPTIONS=()
for i in {1..5}; do
  DATA_TOSS_OPTIONS[i]="-q -i ${ITER_CNT} -t ${THREADS} -l -m proc${i} -a ${ADDRS[i-1]} "
done
PARALLEL_OPTION="--progress"

START=1
MAX_PARALLELS=5

# Setting up program parameters GNU Parallel run
PARALLEL_RUN_CMDS=$(for (( c=$START; c<=$MAX_PARALLELS; c++ )) ; do printf "\"$DATA_TOSS_CMD ${DATA_TOSS_OPTIONS[c]}\" "; done)

# echo $PARALLEL_RUN_OPTION

# Now run it with GNU Parallel!!
CMD="$PARALLEL_CMD $PARALLEL_OPTION ::: $PARALLEL_RUN_CMDS"
eval $CMD
