//
// Created by shernand on 07/06/18.
//

#ifndef PROJECT_OCTOTREE_PARALLEL_H
#define PROJECT_OCTOTREE_PARALLEL_H


#include <vector>
#include <mpi.h>
#include "Particle.h"
#include "Cell.h"


class OctoTree_Parallel {
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

    int getM_prank() const;

    void setM_prank(int m_prank);

    int getM_psize() const;

    void setM_psize(int m_psize);

    MPI_Comm getM_communicator() const;

    void setM_communicator(MPI_Comm m_communicator);


    OctoTree_Parallel(std::vector<Particle> pParticles, double newton_g, double box_size, double timestep, double tree_thres, double softening, MPI_Comm communicator);

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
    /// proc rank
    int m_prank;
    /// communicator size
    int m_psize;
    /// communicator
    MPI_Comm m_communicator;
};


#endif //PROJECT_OCTOTREE_PARALLEL_H
