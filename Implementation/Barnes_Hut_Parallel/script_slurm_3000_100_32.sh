#!/bin/bash
#SBATCH --nodes=2
#SBATCH --ntasks=32
#SBATCH --time=1:50:00
#SBATCH -A phpc2018
#SBATCH --mem-per-cpu=3000

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

mpirun -np 32 ./barnes_hut_par input_options_3000_100_32.txt > bhp_3000_100_32.out


echo Barnes Hut Par FINISHED at `date`
