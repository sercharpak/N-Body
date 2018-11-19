#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --time=0:30:00
#SBATCH -A phpc2018
#SBATCH --mem-per-cpu=2000

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

mpirun -np 4 ./barnes_hut_par input_options_250_100_4.txt > bhp_250_100_4.out


echo Barnes Hut Par FINISHED at `date`
