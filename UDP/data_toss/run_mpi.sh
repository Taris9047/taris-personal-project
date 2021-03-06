#!/bin/sh
PROCESSES=5
PORT=9930
THREADS=3
DEMON=''
QUIET='True'

EXEC=$(command -v ./data_toss)
if [[ -z "$EXEC" ]]; then
  # Try with ./toss
  EXEC=$(command -v ./toss)
  echo "Using $EXEC instead!!"
  # if it fails agian. then exit with error
  if [[ -z "$EXEC" ]]; then
    echo "Can't find executable!!"
    exit -1
  fi
fi

MPIRUN=$(command -v mpirun)
if [[ -z "$MPIRUN" ]]; then
  echo "Can't find mpirun!!"
  exit -1
fi

if [[ -z "$1" ]]; then
  true
else
  PROCESSES=$1
fi

if [[ -z "$2" ]]; then
  true
else
  PORT=$2
fi

if [[ -z "$3" ]]; then
  true
else
  THREADS=$3
fi

if [[ -z "$4" ]]; then
  true
else
  if [[ "$4" == "n" ]]; then
    DEMON=''
  else
    DEMON='-d'
  fi
fi

if [[ -z "$5" ]]; then
  true
else
  if [[ "$5" == "n" ]]; then
    QUIET=''
  else
    QUIET='-q'
  fi
fi

# Finally, run the program...
$MPIRUN -n $PROCESSES $EXEC $PORT $THREADS $DEMON $QUIET
