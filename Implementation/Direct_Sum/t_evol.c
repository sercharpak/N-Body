#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define PI 3.141592653589793238462643383
#define G_GRAV 4.49E-3
#define m 1.0

/**
 * Method to compute the total time of the simulation
 * @param n int number of particles
 * @param n_dim int number of dimensions
 * @return double the total time of the simulation
 */
double totalTimeCalculation(int n, int n_dim){
        double t_dyn;
        double rho;
        double R;
        if(n_dim==3) {
                R = pow(n, 1.0/3.0);
                rho = n / (4.0/3.0) / (PI * pow(R,3));
        }
        else{
                R = pow(n, 1.0/2.0);
                rho = n / (PI * pow(R,2));
        }
        t_dyn = 1.0/sqrt(G_GRAV * rho);

        return 5.0 * t_dyn;
}
/**
 * Method to calculate the time of a one timestep of the simulation
 * @param n int number of particles
 * @param epsilon double smoothing parameter for the force calculation
 * @param n_dim int number of dimensions
 * @return double the time of one timestep of the simulation
 */
double timeStepCalculation(int n, double epsilon, int n_dim){
        double t_dyn;
        double rho;
        if(n_dim==3) {
                rho = n / (4.0/3.0) / (PI * pow(epsilon, 3));
        }
        else{
                rho = n / (PI * pow(epsilon, 2));
        }
        t_dyn = 1.0/sqrt(G_GRAV * rho);
        return t_dyn;
}

/**
 * Method for the calculation of the total energy of the simulation
 * The expensive calculation is the potential energy U (similar to the force)
 * @param p double* the position array
 * @param v double* the velocities array
 * @param U double* the potential energy array
 * @param K double* the kinetic energy array
 * @param n int the number of particles
 * @param n_dim the dimensions simulated
 */
void energyCalculation(double *p, double *v, double *U, double *K, int n, int n_dim){
        int i, j, k;
        double delta_total;
//#pragma omp parallel for private(j, k, delta_total) //Testing OpenMP
        for(i=0; i<n; i++) {
                K[i] = 0.0;
                for(k=0; k<n_dim; k++) {
                        K[i] += 0.5 * pow(v[n_dim*i + k],2.0);
                }
                U[i] = 0.0;
                for(j=0; j<n; j++) {
                        if(i!=j) {
                                delta_total = 0.0;
                                for(k=0; k<n_dim; k++) {
                                        delta_total += pow((p[i*n_dim + k] - p[j*n_dim + k]),2);
                                }
                                U[i] += -G_GRAV/sqrt(delta_total);
                        }
                }
        }
}

/**
 * Method which updates the force (or acceleration) of each particle of the simulation
 * This is the main expensive calculation of one step of the simulation.
 * @param p double* the positions array
 * @param v double* the velocities array
 * @param a double* the accelerations array
 * @param n int the number of particles
 * @param epsilon double the smoothing element for the force calculation. Essential to manage collisions.
 * @param n_dim int the number of dimensions simulated
 */
void forceCalculation(double *p, double *v, double *a, int n, double epsilon, int n_dim){
        int i,j,k;
        double delta, delta_total;
//#pragma omp parallel for private(j, k, delta_total, delta) //Testing OpenMP
        for(i=0; i<n; i++) {
                for(k=0; k<n_dim; k++) {
                        a[i*n_dim + k] = 0.0;
                }
                for(j=0; j<n; j++) {
                        if(i!=j) {
                                delta_total = 0.0;
                                for(k=0; k<n_dim; k++) {
                                        delta_total += pow((p[i*n_dim + k] - p[j*n_dim + k]),2);
                                }
                                for(k=0; k<n_dim; k++) {
                                        delta = p[i*n_dim + k] - p[j*3 + k];
                                        a[i*n_dim + k] += -G_GRAV * delta
                                                          / pow((delta_total + pow(epsilon,2)), 3.0/2.0);
                                }
                        }
                }
        }
}

/**
 * Method which updates the velocities of the simulation (kick) from the accelerations
 * @param p double* the positions array
 * @param v double* the velocities array
 * @param a double* the accelerations array
 * @param n int the number of particles
 * @param delta_t double the parameter of the algorithm. How much we move in time.
 * @param n_dim int the number of dimensions simulated
 */
void  kick(double *p, double *v, double *a, int n, double delta_t, int n_dim){
        int i,k;
//#pragma omp parallel for private(k) //Testing OpenMP
        for(i=0; i<n; i++) {
                for(k=0; k<n_dim; k++) {
                        v[i*n_dim + k] += a[i*n_dim + k] * delta_t;
                }
        }
}

/**
 * Method which updates all the positions of the particles (drift) from their velocities.
 * @param p double* the positions array
 * @param v double* the velocities array
 * @param a double* the accelerations array
 * @param n int the number of particles
 * @param delta_t double the parameter of the algorithm. How much we move in time.
 * @param n_dim int the number of dimensions simulated
 */
void  drift(double *p, double *v, double *a, int n, double delta_t, int n_dim){
        int i,k;
//#pragma omp parallel for private(k) //Testing OpenMP
        for(i=0; i<n; i++) {
                for(k=0; k<n_dim; k++) {
                        p[i*n_dim + k] += v[i*n_dim + k] * delta_t;
                }
        }
}
