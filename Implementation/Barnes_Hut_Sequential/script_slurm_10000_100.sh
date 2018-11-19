#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=2:50:00
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

echo Barnes Hut Seq STARTING at `date`

./barnes_hut_seq input_options_10000_100.txt >  bhs_10000_100.out

echo Barnes Hut Seq FINISHED at `date`
