#!/bin/bash
set -e

if [ "$#" -ne 1 ]
then
  echo "Usage: fuzz.sh [PATH_TO_FUZZ_EXECUTABLE]"
  exit 1
fi

FUZZ_INPUTS="./fuzz_inputs"
FUZZ_OUTPUT="./fuzz_output"
mkdir -p "$FUZZ_OUTPUT"

afl-fuzz -i $FUZZ_INPUTS -o $FUZZ_OUTPUT -M fuzz0 $1 > /dev/null &

for i in $(seq 1 7)
do
  afl-fuzz -i $FUZZ_INPUTS -o $FUZZ_OUTPUT -S "fuzz$i" $1 > /dev/null &
done
