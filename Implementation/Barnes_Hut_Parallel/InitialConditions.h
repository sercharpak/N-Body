//
// Created by sergi on 6/7/2018.
//
/** @file InitialConditions.h
 *  @author Sergio Hernandez
 *
 * This file is part of the N-Body Project for the 2018 PHPC Course.
 *
 *
 * This class performs the methods to compute an initial condition for the algorithm.
 *
 */
#ifndef PROJECT_INITIALCONDITIONS_H
#define PROJECT_INITIALCONDITIONS_H


#include <vector>
#include "Particle.h"

class InitialConditions {
public:
    InitialConditions();
    /**
     * Generates a plummer sphere with N solar masses, scale radius a ni parsec.
     * @param N
     * @param a
     * @param newton_g
     * @return
     */
    static std::vector<Particle> plummerSphere(int N, double a, double newton_g);
private:

};


#endif //PROJECT_INITIALCONDITIONS_H
