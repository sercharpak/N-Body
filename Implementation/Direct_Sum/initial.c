#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.141592653589793238462643383279502884197
#define FLOAT double

/**
 * Reads the arguments entered through the command line.
 * @param argc int number of arguments
 * @param argv char** the argument array
 * @param n int* pointer to the number of particles to be assigned
 * @param e FLOAT* pointer to the epsilon value to be assigned.
 * @param th int* pointer to the thread number to be assigned (OpenMP test). 1 when serial code.
 * @param n_dim int* pointer to the dimension number to be assigned
 */
void readInput(int argc, char **argv, int *n, FLOAT *e, int *th, int *n_dim){
								//This function makes sure the amount of parameters is correct
								//The number of particles and epsilon are stored in pointers
								if(argc!=5) {
																fprintf(stderr, "Wrong number of parameters. USAGE: main.x N epsilon threads n_dim\n");
																exit(1);
								}
								*n = atoi(argv[1]);
								*e = atof(argv[2]);
								*th = atoi(argv[3]);
								*n_dim = atoi(argv[4]);
}

/**
 * Standard method to allocate memory for a vector and initialize its values to 0.0
 * @param n int length of the vector
 * @return FLOAT* the pointer of the vector
 */
FLOAT *createVector(int n){
								//We generate the vector for the position, velocity and acceleration of size n
								//The memory for this vector is allocated and its initialized in zeros
								FLOAT *v;
  int i;
								if(!(v = (double *)malloc(n * sizeof(FLOAT)))) {
																fprintf(stderr, "Problem with memory allocation\n");
																exit(1);
								}

								for(i=0; i < n; i++) {
																v[i] = 0.0;
								}
								return v;
}

/**
 * Generates the initial positions of the simulation
 * @param p FLOAT* the positions of the particles
 * @param n int the number of particles
 * @param n_dim the number of dimensions.
 */
void initialPositions(FLOAT *p, int n, int n_dim){
								//We generate n points randomly distributed in an sphere
								//The function writes the particle's position an velocity (in x, y, and z)

								FLOAT phi, theta, r, costheta;
								FLOAT R;
								int i;

								if(n_dim==3) {
																R = pow(n, 1.0/3.0);
																for(i=0; i<n; i++) {
																								phi = 2.0 * PI * drand48();
																								costheta = 2.0*(drand48() - 0.5);
																								theta  = acos(costheta);
																								r = R * pow(drand48(), 1.0/3.0);

																								p[i*n_dim + 0] = r * cos(phi) * sin(theta);
																								p[i*n_dim + 1] = r * sin(phi) * sin(theta);
																								p[i*n_dim + 2] = r * cos(theta);
																}
								}
								else{
																R = pow(n, 1.0/2.0);
																for(i=0; i<n; i++) {
																								phi = 2.0 * PI * drand48();
																								r = R * pow(drand48(), 1.0/2.0);

																								p[i*n_dim + 0] = r * cos(phi);
																								p[i*n_dim + 1] = r * sin(phi);
																}
								}
}


/**
 * Writes a snapshot of the simulation
 * @param x FLOAT * x[n_particles * n_dim] (Position)
 * @param y FLOAT * y[n_particles * n_dim] (Velocities)
 * @param U FLOAT * U[n_particles] (Potential Energy)
 * @param K FLOAT * K[n_particles] (Kinetic Energy)
 * @param n int n_particles
 * @param id int makes the difference between initial state and final state
 * @param n_dim int the number of dimensions.
 */
void saveState(FLOAT *x, FLOAT *y, FLOAT *U, FLOAT *K, int n, int id, int n_dim){
								//We write the current state of particles in a text file
								//Each row will have the state for a particle r
								//The format is: pos_x \t pos_y \t pos_z \t vel_x \t vel_y \t vel_z
								//The name of the file contains: -state_i (the time for which the name is written - n_particles (the number of particles)

								FILE* saveFile;

								char bufX[20];
								char bufY[20];
								char n1[50], n2[50], n3[50];
								int i;

								//Prepare the name of the file
								if(id==0) {
																strcpy(bufX,  "initial");
								}
								else{
																strcpy(bufX,  "final");
								}
								sprintf(bufY, "%d", n);
								strcpy(n1,  "state_");
								strcpy(n2, "_");
								strcpy(n3, ".dat");
								strcat(n1, bufX);
								strcat(n1, n2);
								strcat(n1, bufY);
								strcat(n1, n3);

								//Opens the file
								saveFile = fopen(n1, "w");


								if(!(saveFile=fopen(n1, "w"))) {
																fprintf(stderr, "Problem opening file %s\n", n1);
																exit(0);
								}

								if(n_dim==3) {


																//Writes in the file
																for(i=0; i<n; i++) {
																								fprintf(saveFile, "%f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \n", x[n_dim*i +0], x[n_dim*i +1], x[n_dim*i +2], y[n_dim*i +0], y[n_dim*i +1], y[n_dim*i +2], U[i], K[i]);
																}
								}
								else{
																fprintf(saveFile, "%f \t %f \t %f \t %f \t %f \t %f \n", x[n_dim*i +0], x[n_dim*i +1], y[n_dim*i +0], y[n_dim*i +1], U[i], K[i]);
								}
								//Closes the file
								fclose(saveFile);

}
