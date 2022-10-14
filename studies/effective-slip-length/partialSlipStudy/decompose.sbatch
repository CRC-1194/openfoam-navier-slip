#!/bin/bash
#SBATCH -J decomposePar 
#SBATCH -n 1 
#SBATCH --mem-per-cpu=1000 
#SBATCH -t 00:10:00     

srun decomposePar
