#!/bin/bash
#SBATCH -J decompose 
#SBATCH -n 1 
#SBATCH --mem-per-cpu=1000 
#SBATCH -t 00:05:00     

srun decomposePar 
