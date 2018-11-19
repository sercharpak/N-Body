//
// Created by sergi on 6/7/2018.
//

#include <ctime>
#include <iostream>
#include <vector>
#include "OctoTree_Serial.h"


OctoTree_Serial::OctoTree_Serial(std::vector<Particle> pParticles, double newton_g, double box_size, double timestep,
                                 double tree_thres, double softening)  : root(-box_size, box_size, -box_size, box_size, -box_size, box_size,"0") {
    setParticles(pParticles);
    setNewton_g(newton_g);
    setBox_size(box_size);
    setTimestep(timestep);
    setTree_thres(tree_thres);
    setSoftening(softening);
    std::vector<double> timings;
    timings.push_back(0.0);
    timings.push_back(0.0);
    setTimings(timings);
    root = Cell(-getBox_size(), getBox_size(), -getBox_size(), getBox_size(), -getBox_size(), getBox_size(),"0");
}

const std::vector<Particle> &OctoTree_Serial::getParticles() const {
    return particles;
}

void OctoTree_Serial::setParticles(const std::vector<Particle> &particles) {
    OctoTree_Serial::particles = particles;
}

double OctoTree_Serial::getNewton_g() const {
    return newton_g;
}

void OctoTree_Serial::setNewton_g(double newton_g) {
    OctoTree_Serial::newton_g = newton_g;
}

double OctoTree_Serial::getBox_size() const {
    return box_size;
}

void OctoTree_Serial::setBox_size(double box_size) {
    OctoTree_Serial::box_size = box_size;
}

double OctoTree_Serial::getTimestep() const {
    return timestep;
}

void OctoTree_Serial::setTimestep(double timestep) {
    OctoTree_Serial::timestep = timestep;
}

double OctoTree_Serial::getTree_thres() const {
    return tree_thres;
}

void OctoTree_Serial::setTree_thres(double tree_thres) {
    OctoTree_Serial::tree_thres = tree_thres;
}

double OctoTree_Serial::getSoftening() const {
    return softening;
}

void OctoTree_Serial::setSoftening(double softening) {
    OctoTree_Serial::softening = softening;
}

const std::vector<double> &OctoTree_Serial::getTimings() const {
    return timings;
}

void OctoTree_Serial::setTimings(const std::vector<double> &timings) {
    OctoTree_Serial::timings = timings;
}

void OctoTree_Serial::evolve() {
    //std::vector<double> timings = getTimings();
    std::clock_t start,end;
    //Tree
    start = std::clock();
    root = Cell(-getBox_size(), getBox_size(), -getBox_size(), getBox_size(), -getBox_size(), getBox_size(),"0");
    for (auto &particle: particles) {
        //std::cout<<"particle: "<< particle.getName()  << std::endl;
        root.add(particle);
    }
    end = std::clock();
    timings[0] = ( end - start ) / (double) CLOCKS_PER_SEC;
    //Force
    particles = root.particles();
//    std::cout<<"Retrieved particles : "<< particles.size()  << std::endl;
//    for (auto particle: particles_retrieved) {
//        std::cout<<"particle retrieved: "<< particle.getName()  << std::endl;
//    }
    start = std::clock();
    //setParticles(particles_retrieved);
    for (auto &particle: particles) {
//       std::cout<<"particle: "<< particle.getName()  << std::endl;
       std::vector<Cell> cells;
       cells.push_back(root);
        while(!cells.empty()){
            Cell cell = cells.back();
            //std::cout<<"Cell: "<< cell.getName()  << std::endl;
            cells.pop_back();
            if(cell.meetsCriterion(particle,getTree_thres(), getSoftening())){
                if(cell.getN()>0){
                    particle.kick(cell,newton_g, getTimestep(), getSoftening());
                }
            }
            else{
                std::vector<Cell> daughterCells = cell.getDaughters();
                cells.insert(cells.end(), daughterCells.begin(), daughterCells.end());
            }
        }
        particle.drift(getTimestep());
    }
    end = std::clock();
    timings[1] += ( end - start ) / (double) CLOCKS_PER_SEC;

}

const Cell &OctoTree_Serial::getRoot() const {
    return root;
}

void OctoTree_Serial::setRoot(const Cell &root) {
    OctoTree_Serial::root = root;
}


