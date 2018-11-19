//=======================================================================
/** @file main_direct.c
 *  @author Sergio Daniel Hernandez
 *
 * Code inspired from the MCA course at Uniandes (2016-1)
 * This file is part of the N-Body simulation project.
 *
 * main_direct.c is the entry point
 *
 */
//=======================================================================

#include <stdio.h>
#include <stdlib.h>
//#include <omp.h>//Testing OpenMP
#include "initial.h"
#include "t_evol.h"

/**
 * Follows the dynamics of the gravitational N-Body Problem
 * with the Particle to particle direct sum. Serial code with the basic OpenMP commented.
 * Main method of the program
 * @param argc int number of arguments received
 * @param argv char* the arguments received.
 * @return int 0 if well executed 1 if not.
 */
int main(int argc, char **argv){
								//declaring and initializing variables
								int i=0, N=0, threads=1; //Testing OpenMP
								double *p=NULL;
								double *v=NULL;
								double *a=NULL;
								double *U;
								double *K;
								double time_step=0, total_time=0, epsilon=0;
								int n_steps=0;
								int n_dim=3; //can be also 3.

								//Reads input
								readInput(argc, argv, &N, &epsilon, &threads, &n_dim);

								//omp_set_num_threads(threads); //Testing OpenMP

								p = createVector(n_dim*N); //positions
								v = createVector(n_dim*N); //velocities
								a = createVector(n_dim*N); //accelerations
								U = createVector(N); //potential energy
								K = createVector(N); //kinetic energy
								initialPositions(p, N, n_dim); //initial positions
								energyCalculation(p, v, U, K, N, n_dim); //energy calculation
								saveState(p, v, U, K, N, i, n_dim); //writes a snapshot

								// characteristic times
								total_time = totalTimeCalculation(N, n_dim); //total time of the simulation
								time_step = timeStepCalculation(N, epsilon, n_dim); //time step of the simulation
								n_steps = (int)(total_time/time_step); //number of steps to perform
								fprintf(stderr, "Total time: %f Time step: %f n_steps %d\n",total_time, time_step, n_steps);

								//temporal evolution

								forceCalculation(p, v, a, N, epsilon, n_dim); //first force calculation
								kick(p, v, a, N, time_step/2.0, n_dim); //first kick of the LEAP algorithm
								for(i=0; i<n_steps; i++) {
																drift(p, v, a, N, time_step, n_dim); //drif of the LEAP algorithm
																forceCalculation(p, v, a, N, epsilon, n_dim); //force calculation
																kick(p, v, a, N, time_step, n_dim); //kick of the LEAP algorithm
								}
								kick(p, v, a, N, -time_step/2.0, n_dim); //last kick of the LEAP algorithm
								energyCalculation(p, v, U, K, N, n_dim); //energy calculation
								saveState(p, v, U, K, N, i, n_dim); //writes a snapshot

								//frees the memory
								free(p);
								free(v);
								free(a);
								free(U);
								free(K);

								return 0;
}
