//
// Created by sergi on 6/7/2018.
//

#ifndef PROJECT_OCTOTREE_SERIAL_H
#define PROJECT_OCTOTREE_SERIAL_H


#include "Particle.h"
#include "Cell.h"

class OctoTree_Serial {
public:
    const std::vector<Particle> &getParticles() const;

    void setParticles(const std::vector<Particle> &particles);

    double getNewton_g() const;

    void setNewton_g(double newton_g);

    double getBox_size() const;

    void setBox_size(double box_size);

    double getTimestep() const;

    void setTimestep(double timestep);

    double getTree_thres() const;

    void setTree_thres(double tree_thres);

    double getSoftening() const;

    void setSoftening(double softening);

    const std::vector<double> &getTimings() const;

    void setTimings(const std::vector<double> &timings);

    const Cell &getRoot() const;

    void setRoot(const Cell &root);

    OctoTree_Serial(std::vector<Particle> pParticles, double newton_g, double box_size, double timestep, double tree_thres, double softening);

    void evolve();
private:
    std::vector<Particle> particles;
    double newton_g;
    double box_size;
    double timestep;
    double tree_thres;
    double softening;
    std::vector<double> timings;
    Cell root;
};


#endif //PROJECT_OCTOTREE_SERIAL_H
