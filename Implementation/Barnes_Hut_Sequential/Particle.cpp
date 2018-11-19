//
// Created by sergi on 6/6/2018.
//
/** @file Particle.cpp
 *  @author Sergio Hernandez
 *
 * This file is part of the N-Body Project for the 2018 PHPC Course.
 *
 *
 * This class performs the methods of handling a particle.
 *
 */
#include <cmath>
#include <tuple>
#include "Particle.h"


double Particle::getX() const {
    return x;
}

void Particle::setX(double x) {
    Particle::x = x;
}

double Particle::getY() const {
    return y;
}

void Particle::setY(double y) {
    Particle::y = y;
}

double Particle::getZ() const {
    return z;
}

void Particle::setZ(double z) {
    Particle::z = z;
}

double Particle::getVx() const {
    return vx;
}

void Particle::setVx(double vx) {
    Particle::vx = vx;
}

double Particle::getVy() const {
    return vy;
}

void Particle::setVy(double vy) {
    Particle::vy = vy;
}

double Particle::getVz() const {
    return vz;
}

void Particle::setVz(double vz) {
    Particle::vz = vz;
}

const std::string &Particle::getName() const {
    return name;
}

void Particle::setName(const std::string &name) {
    Particle::name = name;
}

Particle::Particle(double pX, double pY, double pZ, double pVx, double pVy, double pVz) {
    Particle::setVz(pVz);
    Particle::setVy(pVy);
    Particle::setVx(pVx);
    Particle::setZ(pZ);
    Particle::setY(pY);
    Particle::setX(pX);
    Particle::setName("NoName");
}

Particle::Particle(double pX, double pY, double pZ, double pVx, double pVy, double pVz, std::string pName) {
    Particle::setName(pName);
    Particle::setVz(pVz);
    Particle::setVy(pVy);
    Particle::setVx(pVx);
    Particle::setZ(pZ);
    Particle::setY(pY);
    Particle::setX(pX);
}
#include "Cell.h" //Forward declaration because of the mutual dependency Cell - Particle
void Particle::kick(Cell pCell, double newton_g, double timestep, double softening) {
    //Computing distances
    double rx, ry, rz, r2;
    rx = pCell.getXcen() - x;
    ry = pCell.getYcen() - y;
    rz = pCell.getZcen() - z;
    r2 = rx*rx + ry*ry + rz*rz;

    //Managing collisions (softening)
    if (r2 > (softening*softening)) {
//        double vx = getVx();
//        double vy = getVy();
//        double vz = getVz();
        vx += (newton_g*timestep*rx*pCell.getN())/(pow(r2,1.5));
        vy += (newton_g*timestep*ry*pCell.getN())/(pow(r2,1.5));
        vz += (newton_g*timestep*rz*pCell.getN())/(pow(r2,1.5));
//        setVx(vx);
//        setVy(vy);
//        setVz(vz);
    }
    else{
        double r, s, f;
        r = pow(r2,0.5);
        s = r/softening;
        f = s*(8-9*x+2*s*s*s); //From Dyer, C. C. & Ip, P. S. S. (1993)
//        double vx = getVx();
//        double vy = getVy();
//        double vz = getVz();
        vx += (newton_g*timestep*f*rx*pCell.getN())/(softening*softening*r);
        vy += (newton_g*timestep*f*ry*pCell.getN())/(softening*softening*r);
        vz += (newton_g*timestep*f*rz*pCell.getN())/(softening*softening*r);
//        setVx(vx);
//        setVy(vy);
//        setVz(vz);
    }
}

void Particle::drift(double timestep) {
//    double vx = getVx();
//    double vy = getVy();
//    double vz = getVz();
//    setX(getX()+vx*timestep);
//    setY(getY()+vy*timestep);
//    setZ(getZ()+vz*timestep);
    x += vx*timestep;
    y += vy*timestep;
    z += vz*timestep;
}

bool Particle::operator==(const Particle &rhs) const {
    //return (getX() == rhs.getX()) && (getY() == rhs.getY()) && (getZ() == rhs.getZ()); //Just with the coordinates
    return (getName()== rhs.getName()); //Just with the name
}

bool Particle::operator!=(const Particle &rhs) const {
    //return (getX() != rhs.getX()) || (getY() != rhs.getY()) || (getZ() != rhs.getZ()); //Just with the coordinates
    return (getName()!= rhs.getName()); //Just with the name
}

