//
// Created by sergi on 6/7/2018.
//

#include <cmath>
#include <iostream>
#include "InitialConditions.h"

InitialConditions::InitialConditions() {}

std::vector<Particle> InitialConditions::plummerSphere(int N, double a, double newton_g) {
    std::vector<Particle> particles;
    std::vector<double> x,y,z;
    std::vector<double> vx, vy, vz;
    int n_stars = 0;
    while (n_stars<N){
        double random_radius = drand48();
        double radius = a/sqrt(pow(random_radius,(-2.0/3.0))-1);
        double xx = drand48();
        double yy = drand48()*0.1;
        if(yy<(xx*xx*pow((1-xx*xx),3.5))){
            n_stars+=1;
            double v_mag = xx * sqrt(2*newton_g*N)*pow((radius*radius+a*a),-0.25);
            double phi = drand48()*2.0*M_PI;
            double theta = acos(drand48()*2-1);
            x.push_back(radius*sin(theta)*cos(phi));
            y.push_back(radius*sin(theta)*sin(phi));
            z.push_back(radius*cos(theta));
            phi = drand48()*2.0*M_PI;
            theta = acos(drand48()*2-1);
            vx.push_back(v_mag*sin(theta)*cos(phi));
            vy.push_back(v_mag*sin(theta)*sin(phi));
            vz.push_back(v_mag*cos(theta));
        }
    }
    for(int i=0;i<N;i++){
        //std::cout<<"Particle"<< std::to_string(i)<<" coord : " << x[i] <<y[i]<< z[i]<< std::endl;
        Particle particle = Particle(x[i],y[i],z[i], vx[i], vy[i], vz[i], std::to_string(i));
        particles.push_back(particle);
    }
    return particles;
}
