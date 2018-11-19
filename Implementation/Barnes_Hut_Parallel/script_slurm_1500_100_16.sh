#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=16
#SBATCH --time=0:20:00
#SBATCH -A phpc2018

# mail alert at end of execution
#SBATCH --mail-type=END

# send mail to this address
#SBATCH --mail-user=sergio.hernandez@epfl.ch

#	Script running the nbody simulations
#	output on .dat and .out files 
#	

echo 'Loading modules'

module purge
module load gcc openmpi


# ************************************************
# **             Barnes Hut             **
# ************************************************

echo Barnes Hut Par STARTING at `date`

mpirun -np 16 barnes_hut_par input_options_1500_100_16.txt > bhp_1500_100_16.out


echo Barnes Hut Par FINISHED at `date`
