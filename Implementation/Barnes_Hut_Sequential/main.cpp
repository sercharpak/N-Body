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
#include "OctoTree_Serial.h"
#include "Writter.h"
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
    if(argc!=N_ARGS){
        std::cout<<USAGE<< std::endl;
        return 1;
    }
    try{
        std::string fName = argv[1];
        ConfigFileParser parser = ConfigFileParser();
        parser.parseFile(fName);
        parser.verify();
        std::cout<<"Finished parsing the file " << std::endl;
        //This could be an executer
        std::map<std::string,std::string> data = parser.getData();
        //Number of particles
        std::string tempKey = "n_particles";
        int N = stoi(data[tempKey]);
        tempKey = "n_time_steps";
        int n_time_steps = stoi(data[tempKey]);
        tempKey = "scale_radius";
        double a = stof(data[tempKey]);
        tempKey = "tree_thres";
        double tree_thres = stof(data[tempKey]);
        tempKey = "softening";
        double softening = stof(data[tempKey]);
        tempKey = "newton_g";
        double newton_g = stof(data[tempKey]);
        tempKey = "timestep";
        double timestep = stof(data[tempKey]);
        tempKey = "box_size";
        double box_size = stof(data[tempKey]);
        tempKey = "type_input";
        std::string type_input = data[tempKey];
        tempKey = "type_output";
        std::string type_output = data[tempKey];
        tempKey = "outputFile";
        std::string outputFile = data[tempKey];
        tempKey = "number_outputs";
        int number_outputs = stoi(data[tempKey]);

        std::cout<<"type_input: " << type_input << std::endl;
        std::cout<<"type_output: " << type_output << std::endl;
        std::cout<<"outputFile: " << outputFile << std::endl;
        std::cout<<"box_size: " << box_size << std::endl;
        std::cout<<"timestep: " << timestep << std::endl;
        std::cout<<"newton_g: " << newton_g << std::endl;
        std::cout<<"softening: " << softening << std::endl;
        std::cout<<"tree_thres: " << tree_thres << std::endl;
        std::cout<<"scale_radius: " << a << std::endl;
        std::cout<<"n_time_steps: " << n_time_steps << std::endl;
        std::cout<<"n_particles: " << N << std::endl;

        std::vector<Particle> particles;
        //if(type_input.compare("Plummer")){
        if(type_input=="Plummer"){
            std::cout<<"Initializing particles with: " << type_input << std::endl;
            particles = InitialConditions::plummerSphere(N,a,newton_g);
        }
        if(type_output=="text"){
            Writter::writePositionsText(outputFile,0,particles);
        }

//        for (auto pParticle: particles) {
//            std::cout<<"Particle"<< pParticle.getName()<<" coord : " << pParticle.getX() <<pParticle.getY()<< pParticle.getZ()<< std::endl;
//        }

        int counter_outputs = 1;
        int step_writing = n_time_steps/number_outputs;
//        std::cout<<"step_writing: " <<step_writing<<std::endl;

        OctoTree_Serial nbody = OctoTree_Serial(particles, newton_g, box_size, timestep, tree_thres, softening);
        for (int i = 0 ; i<n_time_steps;i++){
            nbody.evolve();
            if(i==(counter_outputs*step_writing)){
              if(type_output=="text"){
                  Writter::writePositionsText(outputFile,i,particles);
              }
              counter_outputs+=1;
            }
        }
        std::vector<double> timings = nbody.getTimings();
        for(int j=0;j<timings.size();j++){
            std::cout<<"Timing: "<< j << " is "<< timings[j] << std::endl;
        }

        particles = nbody.getParticles();
        if(type_output=="text"){
            Writter::writePositionsText(outputFile,1,particles);
        }
        return 0;
    }
    catch(const std::runtime_error &e){
        std::cout << e.what() <<std::endl;
        return 1;
    }
}
