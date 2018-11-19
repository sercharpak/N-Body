/**@file main.cpp
*  @author Sergio Daniel Hernandez
 *
 * This file is part of the N-Body Project for the 2018 PHPC Course
 *
 * main.cpp is the entry point
 *
 */
class Particle;
class Cell;
#include "ConfigFileParser.h"
#include "Particle.h"
#include "Cell.h"
#include "InitialConditions.h"
#include "Writter.h"
#include "OctoTree_Parallel.h"
/* -------------------------------------------------------------------------- */
#include <mpi.h>
#include <math.h>
/* -------------------------------------------------------------------------- */
/**
 * Constant to inform the user the correct usage of the program in case of a misuse.
 */
#define USAGE "Please enter correctly one argument (a config file). Example: ./Project configFile.txt"
/**
 * Constant which defines the correct number of arguments the program can receive.
 */
#define N_ARGS 2
/**
 * Main method of the program
 * @param argc int number of arguments received
 * @param argv char* the arguments received.
 * @return int 0 if well executed 1 if not.
 */
int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int prank, psize;

    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);
    if(argc!=N_ARGS){
        std::cout<<USAGE<< std::endl;
        return 1;
    }
    try{

        int N, n_time_steps,number_outputs;
        double a, tree_thres, softening, newton_g, timestep,box_size;
        std::string type_input, type_output, outputFile;
        if( prank == 0) {
            //Only the root reads the config file

            std::string fName = argv[1];
            ConfigFileParser parser = ConfigFileParser();
            parser.parseFile(fName);
            parser.verify();
            std::cout << "Finished parsing the file " << std::endl;
            //This could be an executer
            std::map<std::string, std::string> data = parser.getData();
            //Number of particles
            std::string tempKey = "n_particles";
            N = stoi(data[tempKey]);
            tempKey = "n_time_steps";
            n_time_steps = stoi(data[tempKey]);
            tempKey = "scale_radius";
            a = stof(data[tempKey]);
            tempKey = "tree_thres";
            tree_thres = stof(data[tempKey]);
            tempKey = "softening";
            softening = stof(data[tempKey]);
            tempKey = "newton_g";
            newton_g = stof(data[tempKey]);
            tempKey = "timestep";
            timestep = stof(data[tempKey]);
            tempKey = "box_size";
            box_size = stof(data[tempKey]);
            tempKey = "type_input";
            type_input = data[tempKey];
            tempKey = "type_output";
            type_output = data[tempKey];
            tempKey = "outputFile";
            outputFile = data[tempKey];
            tempKey = "number_outputs";
            number_outputs = stoi(data[tempKey]);




        }

//std::cout<<"M_PI "<<M_PI<<std::endl;

        //Here Broadcoast the general parameters
        MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD); //To be sure everybody has everything
        MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&tree_thres, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&softening, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&newton_g, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&timestep, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&box_size, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&n_time_steps, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD); //To be sure everybody has everything

        std::cout << "type_input: " << type_input << std::endl;
        std::cout << "type_output: " << type_output << std::endl;
        std::cout << "outputFile: " << outputFile << std::endl;
        std::cout << "box_size: " << box_size << std::endl;
        std::cout << "timestep: " << timestep << std::endl;
        std::cout << "newton_g: " << newton_g << std::endl;
        std::cout << "softening: " << softening << std::endl;
        std::cout << "tree_thres: " << tree_thres << std::endl;
        std::cout << "scale_radius: " << a << std::endl;
        std::cout << "n_time_steps: " << n_time_steps << std::endl;
        std::cout << "n_particles: " << N << std::endl;



        //Now creates the initial conditions and broadcoast them
        std::vector<Particle> particles; //Our original Particle
        //MPI does not work with objects.
        std::vector<double> x;
        std::vector<double> y;
        std::vector<double> z;
        std::vector<double> vx;
        std::vector<double> vy;
        std::vector<double> vz;

        if( prank == 0) {

            //if(type_input.compare("Plummer")){ //Strange difference between windows and linux
            if (type_input == "Plummer") {
                std::cout << "Initializing particles with: " << type_input << std::endl;
                particles = InitialConditions::plummerSphere(N, a, newton_g);
                //Broadcoast the particles
                //MPI cannot send objects. Have to make std::vector and send them one at the time.
                for (auto &particle: particles) {
                    x.push_back(particle.getX());
                    y.push_back(particle.getY());
                    z.push_back(particle.getZ());
                    vx.push_back(particle.getVx());
                    vy.push_back(particle.getVy());
                    vz.push_back(particle.getVz());
                }
            }
        }

        //Resize the vectors to receive the info
        if (prank != 0) {
            x.resize(N);
            y.resize(N);
            z.resize(N);
            vx.resize(N);
            vy.resize(N);
            vz.resize(N);
            //particles.resize(N);//Apparently not necessary
        }
        //Does the broadcoasts
        MPI_Bcast(&x[0], x.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&y[0], y.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&z[0], z.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&vx[0], vx.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&vy[0], vy.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&vz[0], vz.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD); //To be sure everybody has everything
        //Recreate the particle vector
        if (prank != 0) {
            //std::cout<<"PROCESSOR "<<prank<<std::endl;
            for(int i=0;i<N;i++){
                //std::cout<<"Particle "<< std::to_string(i)<<" coord : " << x[i] <<y[i]<< z[i]<< std::endl;
                Particle particle = Particle(x[i],y[i],z[i], vx[i], vy[i], vz[i], std::to_string(i));
                particles.push_back(particle);
            }
        }


        if (prank == 0) {
            if (type_output == "text") {
                Writter::writePositionsText(outputFile, 0, particles);
            }
        }
        int counter_outputs = 1;
        int step_writing = n_time_steps/number_outputs;

        //Serial implementation
        //OctoTree_Serial nbody = OctoTree_Serial(particles, newton_g, box_size, timestep, tree_thres, softening);
        // Parallel force calculation
        OctoTree_Parallel nbody = OctoTree_Parallel(particles, newton_g, box_size, timestep, tree_thres, softening, MPI_COMM_WORLD);
//        MPI_Barrier(MPI_COMM_WORLD); //To be sure everybody has everything
        std::cout<<"P"<<prank<<" has particles: "<<N<<std::endl;
        std::cout<<"P"<<prank<<" has n_time_steps "<<n_time_steps<<std::endl;

        for (int i = 0 ; i<n_time_steps;i++){
            std::cout<<"P"<<prank<<" time step "<<i<<std::endl;
            MPI_Barrier(MPI_COMM_WORLD);
            //    MPI_Barrier(MPI_COMM_WORLD); //To be sure everybody has everything
            nbody.evolve();
            if(i==(counter_outputs*step_writing)){
                if (prank == 0) {

                    if (type_output == "text") {
                        Writter::writePositionsText(outputFile, i, particles);
                    }
                    counter_outputs += 1;
                }
            }
//            MPI_Barrier(MPI_COMM_WORLD); //To be sure everybody has everything
        }
//        std::cout<<"P"<<prank<<" has END particles "<<particles.size()<<std::endl;


        std::vector<double> timings = nbody.getTimings();
        for(int j=0;j<timings.size();j++){
            std::cout<<"P"<<prank<<" Timing: "<< j << " is "<< timings[j] << std::endl;
        }

        //Sum up Timings
        // Reduce all of the local sums into the global sum
        double global_sum [3];
        MPI_Reduce(&timings[0], &global_sum[0], 3, MPI_DOUBLE, MPI_SUM, 0,
                   MPI_COMM_WORLD);



        if( prank == 0) {

            std::vector<double> timings = nbody.getTimings();
            for(int j=0;j<timings.size();j++){
                std::cout<<"Gobal Times"<<" Timing: "<< j << " is "<< global_sum[j] << std::endl;
            }

            particles = nbody.getParticles();
            if (prank == 0) {
            if (type_output == "text") {
                Writter::writePositionsText(outputFile, 1, particles);
            }}
        }
        MPI_Finalize();
        return 0;
    }
    catch(const std::runtime_error &e){
        std::cout << e.what() <<std::endl;
        MPI_Finalize();
        return 1;
    }
}
