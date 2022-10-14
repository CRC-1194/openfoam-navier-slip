#!/bin/bash
#SBATCH -J foamLog 
#SBATCH -n 1 
#SBATCH --mem-per-cpu=1000 
#SBATCH -t 00:05:00     

srun foam-log-slurm-files.sh meshDensity_00
