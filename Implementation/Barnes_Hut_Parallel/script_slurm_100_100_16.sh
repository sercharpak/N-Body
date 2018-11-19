#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=16
#SBATCH --time=0:20:00
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

mpiexec -np 16 /home/shernand/PHPC/phpc_SDHC_nbody/N-Body_SDHC/Barnes_Hut_Parallel/barnes_hut_par /home/shernand/PHPC/phpc_SDHC_nbody/N-Body_SDHC/Barnes_Hut_Parallel/input_options_100_100_16.txt > bhp_100_100_16.out


echo Barnes Hut Par FINISHED at `date`
