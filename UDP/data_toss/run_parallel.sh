#!/bin/sh -e

# Checking if GNU parallel exists
command -v parallel > /dev/null 2>&1 || { echo >&2 "We need GNU Parallel installed!! Aborting!!"; exit 1; }
PARALLEL_CMD=$(command -v parallel)
echo "GNU Parallel found at $PARALLEL_CMD"

# Checking if ./data_toss exists
command -v $PWD/data_toss > /dev/null 2>&1 || { echo >&2 "We couldn't find data_toss executable!! Trying to compile!!"; make release; }
DATA_TOSS_CMD=$(command -v $PWD/data_toss)
echo "data_toss found at $DATA_TOSS_CMD"

echo ""

# Ok, let's run it with parallel!!
DATA_TOSS_OPTION="-d -q -t 5"
PARALLEL_OPTION="--progress"

START=1
MAX_PARALLELS=5
if [[ -z "$1" ]]; then
  true
else
  MAX_PARALLELS=$1
fi

# Setting up program parameters GNU Parallel run
PARALLEL_RUN_OPTION=$(for (( c=$START; c<=$MAX_PARALLELS; c++ )); do printf "\"$DATA_TOSS_CMD $DATA_TOSS_OPTION\" "; done)
# echo "Parallel program parameter options: $PARALLEL_RUN_OPTION"

# Now run it with GNU Parallel!!
CMD="$PARALLEL_CMD $PARALLEL_OPTION ::: $PARALLEL_RUN_OPTION"
eval $CMD

# CMD=$(for (( c=$START; c<=$MAX_PARALLELS; c++)); do printf "$DATA_TOSS_CMD $DATA_TOSS_OPTION &"; done)
# CMD=${CMD%?}
# eval $CMD
