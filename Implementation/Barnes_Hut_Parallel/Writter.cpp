//
// Created by shernand on 07/06/18.
//

#include <cstring>
#include "Writter.h"

void Writter::writePositionsText(std::string fileName, int id, std::vector<Particle> particles){
    //We write the current state of particles in a text file
    //Each row will have the state for a particle r
    //The format is: pos_x \t pos_y \t pos_z
    //The name of the file contains: -state_i (the time for which the name is written - n_particles (the number of particles)

    FILE* saveFile;

    char bufX[20];
    char bufY[20];
    char n0[100], n1[50], n2[50], n3[50];
    int i;
    int n = particles.size();

    //Prepare the name of the file
    if(id==0) {
        strcpy(bufX,  "initial");
    }
    else if(id==1){
        strcpy(bufX,  "final");
    }
    else{
        std::string s = std::to_string(id);
        strcpy(bufX,  s.c_str());
    }
    sprintf(bufY, "%d", n);
    strcpy(n0,  fileName.c_str());
    strcpy(n1,  "_state_");
    strcpy(n2, "_");
    strcpy(n3, ".dat");
    strcat(n0, n1);
    strcat(n0, bufX);
    strcat(n0, n2);
    strcat(n0, bufY);
    strcat(n0, n3);

    //Opens the file
    saveFile = fopen(n0, "w");


    if(!(saveFile=fopen(n0, "w"))) {
        fprintf(stderr, "Problem opening file %s\n", n0);
        exit(0);
    }

           //Writes in the file
    for (auto &particle: particles) {
            fprintf(saveFile, "%f \t %f \t %f \n", particle.getX(), particle.getY(), particle.getZ());
        }

    //Closes the file
    fclose(saveFile);

}
