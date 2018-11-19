#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=16
#SBATCH --time=3:50:00
#SBATCH -A phpc2018
#SBATCH --mem-per-cpu=4000

# mail alert at end of execution
#SBATCH --mail-type=END

# send mail to this address
#SBATCH --mail-user=sergio.hernandez@epfl.ch

#	Script running the nbody simulations
#	output on .dat and .out files 
#	

echo 'Loading modules'

module purge
module load gcc/7.1.0  openmpi


# ************************************************
# **             Barnes Hut             **
# ************************************************

echo Barnes Hut Par STARTING at `date`

mpirun -np 16 ./barnes_hut_par input_options_100000_100_16.txt > bhp_100000_100_16.out


echo Barnes Hut Par FINISHED at `date`
