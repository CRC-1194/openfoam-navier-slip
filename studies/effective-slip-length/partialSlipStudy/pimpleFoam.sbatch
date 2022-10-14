#!/bin/bash
#SBATCH -J pimpleFoam  
#SBATCH -n 4 
#SBATCH --mem-per-cpu=1000 
#SBATCH -t 02:00:00     

srun pimpleFoam -parallel 
