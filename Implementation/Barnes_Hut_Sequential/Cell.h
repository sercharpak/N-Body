//
// Created by sergi on 6/6/2018.
//
/** @file Cell.h
 *  @author Sergio Hernandez
 *
 * This file is part of the N-Body Project for the 2018 PHPC Course.
 *
 *
 * This class performs the methods of handling a Cell of the tree.
 *
 */
#ifndef PROJECT_CELL_H
#define PROJECT_CELL_H

#include <string>
#include <vector>
#include "Particle.h"


class Cell {

public:

    /**
     * Standard constructor with the limits of the cell and its name
     * @param pXmin
     * @param pXmax
     * @param pYmin
     * @param pYmax
     * @param pZmin
     * @param pZmax
     * @param pName
     */
    Cell(double pXmin, double pXmax, double pYmin, double pYmax, double pZmin, double pZmax, std::string pName);

    double getXmin() const;

    void setXmin(double xmin);

    double getXmax() const;

    void setXmax(double xmax);

    double getYmin() const;

    void setYmin(double ymin);

    double getYmax() const;

    void setYmax(double ymax);

    double getZmin() const;

    void setZmin(double zmin);

    double getZmax() const;

    void setZmax(double zmax);

    const std::string &getName() const;

    void setName(const std::string &name);

    double getXcen() const;

    void setXcen(double xcen);

    double getYcen() const;

    void setYcen(double ycen);

    double getZcen() const;

    void setZcen(double zcen);

    int getN() const;

    void setN(int n);

    const Particle &getParticle() const;

    void setParticle(const Particle &particle);

    bool isOccupied() const;

    void setOccupied(bool occupied);

    bool isHasDaughters() const;

    void setHasDaughters(bool hasDaughters);

    const std::vector<Cell> &getDaughters() const;

    /**
     * Replace the daughters. Recomputes the center of mass coordinates
     * @param daughters
     */
    void setDaughters(const std::vector<Cell> &daughters);

    /**
     * Computes the center of mass
     */
    void computeCenterOfMass();
    /**
     * Verifies if the coordinates are in the cell
     * @param x
     * @param y
     * @param z
     * @return
     */
    bool isInCell(double x, double y, double z);
/**
 * Adds a particle to the tree
 * @param pParticle
 */
    void add(Particle pParticle);
/**
 * Creates the daughters of the octotree cell
 */
    void createDaughters();

    /**
     * Traverse the tree and returns a vector of all the particles
     * @return
     */
    std::vector<Particle> particles();

    /**
     * Verifies if a particle is far enough to use the tree approximation
     * @param pParticle
     * @param tree_thres
     * @param softening
     * @return
     */
    bool meetsCriterion(Particle pParticle,double tree_thres, double softening);
private:
    /**
     * Coordinate xmin
     */
    double xmin;
    /**
     * Coordinate xmax
     */
    double xmax;
    /**
     * Coordinate ymin
     */
    double ymin;
    /**
     * Coordinate ymax
     */
    double ymax;
    /**
     * Coordinate zmin
     */
    double zmin;
    /**
     * Coordinate zmax
     */
    double zmax;
    /**
 * Particle name
 */
    std::string name;
    /**
     * Coordinate xcen
     */
    double xcen;
    /**
     * Coordinate ycen
     */
    double ycen;
    /**
     * Coordinate zcen
     */
    double zcen;
    /**
     * Number of particles
     */
    int n;
    /**
     * Boolean which indicates the cell has a particle
     */
    bool occupied;
    /**
     * Boolean which indicates if the cell has daughters
     */
    bool hasDaughters;
    /**
     * Assigned Particle
     */
    Particle particle;// = Particle::Particle(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);  //TODO - Not sure if this is a good way.
    /**
     * Daughter cells
     */
    std::vector<Cell> daughters;


};


#endif //PROJECT_CELL_H
