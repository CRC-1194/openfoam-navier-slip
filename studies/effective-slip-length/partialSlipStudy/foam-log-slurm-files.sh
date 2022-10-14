#!/usr/bin/env bash

PATTERN=$1

if [ -z "$PATTERN" ]; 
then 
    echo ERROR: No pattern provided for the directories to be searched for SLURM .out files
fi

for case in "$PATTERN"*;
do
   cd $case  
   LAST_SLURM_FILE=$(ls -t *.out | head -1)
   foamLog $LAST_SLURM_FILE
   cd .. 
done

