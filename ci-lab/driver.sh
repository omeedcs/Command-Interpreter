#!/bin/bash

# C S 429 EEL interpreter
#
# driver.sh - Testing script for the EEL interpreter.
# 
# Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
# May not be used, modified, or copied without permission.

set -e

TESTFILE=$1
if [ ! -f "$TESTFILE" ]; then
    echo "$TESTFILE does not exist"
    exit 1
fi

./ci -i $TESTFILE -o _output1
./ci_reference -i $1 -o _output2
if [ ! -f _output1 ]; then
    echo "ci failed to create the output file"
    exit 1
fi
if [ ! -f _output2 ]; then
    echo "ci_reference failed to create the output file"
    exit 1
fi

DIFFS=$(grep -nvxf _output1 _output2 | cut -d ":" -f 1)
if [ "$DIFFS" ] 
then
    echo "failed testcases:"
    echo -e "id | expression | output | gold"
    for line in $DIFFS
    do
    EXP=$(sed -n "${line}p" $TESTFILE)
    OUTPUT=$(sed -n "${line}p" _output1)
    GOLD=$(sed -n "${line}p" _output2)
    echo -e "$line | $EXP | $OUTPUT | $GOLD"
    done
else
    echo "all testcases passed"
fi

rm -f _output1 _output2
