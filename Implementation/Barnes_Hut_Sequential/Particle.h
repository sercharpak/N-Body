//
// Created by sergi on 6/6/2018.
//
/** @file Particle.h
 *  @author Sergio Hernandez
 *
 * This file is part of the N-Body Project for the 2018 PHPC Course.
 *
 *
 * This class performs the methods of handling a particle.
 *
 */
#ifndef PROJECT_PARTICLE_H
#define PROJECT_PARTICLE_H

#include <string>
class Cell; //Forward declaration because of the mutual dependency Cell - Particle

class Particle {
public:
    double getX() const;

    void setX(double x);

    double getY() const;

    void setY(double y);

    double getZ() const;

    void setZ(double z);

    double getVx() const;

    void setVx(double vx);

    double getVy() const;

    void setVy(double vy);

    double getVz() const;

    void setVz(double vz);

    const std::string &getName() const;

    void setName(const std::string &name);

/**
 * Constructor with the coordinates and velocities
 * @param pX
 * @param pY
 * @param pZ
 * @param pVx
 * @param pVy
 * @param pVz
 */
    Particle(double pX, double pY, double pZ, double pVx, double pVy, double pVz);
/**
 * Standard constructor with the coordinates, velocities and name
 * @param pX
 * @param pY
 * @param pZ
 * @param pVx
 * @param pVy
 * @param pVz
 * @param pName
 */
    Particle(double pX, double pY, double pZ, double pVx, double pVy, double pVz, std::string pName);
/**
 * Kick of the LEAP algorithm
 * @param pCell
 * @param newton_g
 * @param timestep
 * @param softening
 */
    void kick(Cell pCell, double newton_g, double timestep, double softening);
/**
 * Drift of the LEAP algorithm
 * @param timestep
 */
    void drift(double timestep);
/**
 * Overriding the compare operator
 * @param rhs
 * @return
 */
    bool operator==(const Particle& rhs) const;
    /**
 * Overriding the compare operator
 * @param rhs
 * @return
 */
    bool operator!=(const Particle& rhs) const;
private:
    /**
     * Coordinate x
     */
    double x;
    /**
     * Coordinate y
     */
    double y;
    /**
     * Coordinate z
     */
    double z;
    /**
     * Velocity in x
     */
    double vx;
    /**
     * Velocity in y
     */
    double vy;
    /**
     * Velocity in z
     */
    double vz;
    /**
     * Particle name
     */
    std::string name;

};


#endif //PROJECT_PARTICLE_H
