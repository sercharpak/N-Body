#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=2
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

mpirun -np 2 ./barnes_hut_par input_options_1000_100_2.txt > bhp_1000_100_2.out


echo Barnes Hut Par FINISHED at `date`
