//
// Created by sergi on 6/6/2018.
//
/** @file Cell.cpp
 *  @author Sergio Hernandez
 *
 * This file is part of the N-Body Project for the 2018 PHPC Course.
 *
 *
 * This class performs the methods of handling a Cell of the tree.
 *
 */

#include "Cell.h"

#include <cmath>
#include <iostream>

double Cell::getXmin() const {
    return xmin;
}

void Cell::setXmin(double xmin) {
    Cell::xmin = xmin;
}

double Cell::getXmax() const {
    return xmax;
}

void Cell::setXmax(double xmax) {
    Cell::xmax = xmax;
}

double Cell::getYmin() const {
    return ymin;
}

void Cell::setYmin(double ymin) {
    Cell::ymin = ymin;
}

double Cell::getYmax() const {
    return ymax;
}

void Cell::setYmax(double ymax) {
    Cell::ymax = ymax;
}

double Cell::getZmin() const {
    return zmin;
}

void Cell::setZmin(double zmin) {
    Cell::zmin = zmin;
}

double Cell::getZmax() const {
    return zmax;
}

void Cell::setZmax(double zmax) {
    Cell::zmax = zmax;
}

const std::string &Cell::getName() const {
    return name;
}

void Cell::setName(const std::string &name) {
    Cell::name = name;
}

double Cell::getXcen() const {
    return xcen;
}

void Cell::setXcen(double xcen) {
    Cell::xcen = xcen;
}

double Cell::getYcen() const {
    return ycen;
}

void Cell::setYcen(double ycen) {
    Cell::ycen = ycen;
}

double Cell::getZcen() const {
    return zcen;
}

void Cell::setZcen(double zcen) {
    Cell::zcen = zcen;
}

int Cell::getN() const {
    return n;
}

void Cell::setN(int n) {
    Cell::n = n;
}

const Particle &Cell::getParticle() const {
    return particle;
}

void Cell::setParticle(const Particle &particle) {
    Cell::particle = particle;
}

bool Cell::isOccupied() const {
    return occupied;
}

void Cell::setOccupied(bool occupied) {
    Cell::occupied = occupied;
}
bool Cell::isHasDaughters() const {
    return hasDaughters;
}

void Cell::setHasDaughters(bool hasDaughters) {
    Cell::hasDaughters = hasDaughters;
}
const std::vector<Cell> &Cell::getDaughters() const {
    return daughters;
}

void Cell::setDaughters(const std::vector<Cell> &daughters) {
    Cell::daughters = daughters;
    int n = 0;
    for (auto daughter: daughters) n += daughter.getN();
    setN(n);
    if(n>0){
      computeCenterOfMass();
    }
    setHasDaughters(true);
}
//Manage https://stackoverflow.com/questions/34279385/c-field-object-var-must-be-initialized-in-constructor-of-base-class
Cell::Cell(double pXmin, double pXmax, double pYmin, double pYmax, double pZmin, double pZmax, std::string pName)  : particle(0, 0, 0, 0, 0, 0){
    Cell::setName(pName);
    Cell::setZmax(pZmax);
    Cell::setZmin(pZmin);
    Cell::setYmax(pYmax);
    Cell::setYmin(pYmin);
    Cell::setXmax(pXmax);
    Cell::setXmin(pXmin);
    Cell::setN(0);
    Cell::setZcen(0.0);
    Cell::setYcen(0.0);
    Cell::setXcen(0.0);
    //TODO - Not sure if this is a good way to initialize at nothing. Maybe a standard init would be better
    Particle  particle = Particle(0, 0, 0, 0, 0, 0);
    Cell::setParticle(particle);
    setOccupied(false);
    setHasDaughters(false);
}

void Cell::computeCenterOfMass() {
    double xcen = 0.0;
    double ycen = 0.0;
    double zcen = 0.0;
    std::vector<Cell> daughters = getDaughters();
    for (auto daughter: daughters) {
        xcen += daughter.getN() * daughter.getXcen();
        ycen += daughter.getN() * daughter.getYcen();
        zcen += daughter.getN() * daughter.getZcen();
    }
    setXcen(xcen/getN());
    setYcen(ycen/getN());
    setZcen(zcen/getN());
}

bool Cell::isInCell(double x, double y, double z) {
    if((x>getXmin())&&(x<=getXmax())&&(y>getYmin())&&(y<=getYmax())&&(z>getZmin())&&(z<=getZmax())){
        return true;
    }
    else{
        return false;
    }
}

void Cell::add(Particle pParticle) {
//    std::cout<<"Trying to add particle : " << pParticle.getName() <<" to Cell "<< getName()<< std::endl;
//    std::cout<<"Particle"<< pParticle.getName()<<" coord : " << pParticle.getX() <<pParticle.getY()<< pParticle.getZ()<< std::endl;
//    std::cout<<"Cell"<< getName()<<" coord min: " << getXmin()<<getYmin()<< getZmin()<< std::endl;
//    std::cout<<"Cell"<< getName()<<" coord max: " << getXmax()<<getYmax()<< getZmax()<< std::endl;
 if(isInCell(pParticle.getX(), pParticle.getY(), pParticle.getZ())){
     //std::cout<<"Is in the cell box: particle : " << pParticle.getName() <<" to Cell "<< getName()<< std::endl;
     if(getN()>0){
         if(isOccupied()){//Needs to pass its particle to daughters and create daughters

             createDaughters();
             //std::vector<Cell> daughters_new;
             for (auto &daughter: daughters) {
                 daughter.add(particle);
                 //daughters_new.push_back(daughter);
             }
             //daughters=daughters_new;
             setOccupied(false);
             setParticle(Particle(0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
         }
         //std::vector<Cell> daughters = getDaughters();
         //std::vector<Cell> daughters_new;
         for (auto &daughter: daughters) {
             //std::cout <<"Daughter Before: " << daughter.getName() <<" has  "<< daughter.getN()<< std::endl;
             daughter.add(pParticle);
             //daughters_new.push_back(daughter);
             //std::cout <<"Daughter After : " << daughter.getName() <<" has  "<< daughter.getN()<< std::endl;
         }
         //daughters=daughters_new;
     }
     else{
         //std::cout<<"Added particle : " << pParticle.getName() <<" to Cell "<< getName()<< std::endl;
         setOccupied(true);
         setParticle(pParticle);
     }
     //Change in the center of mass
     setXcen((getN()*getXcen()+pParticle.getX())/(getN()+1.0));
     setYcen((getN()*getYcen()+pParticle.getY())/(getN()+1.0));
     setZcen((getN()*getZcen()+pParticle.getZ())/(getN()+1.0));
     n+=1;
//     std::cout<<" Cell "<< getName()<<" has  "<< n<< std::endl;
//
//     for (auto daughter: daughters) {
//         std::cout <<"Daughter End: " << daughter.getName() <<" has  "<< daughter.getN()<< std::endl;
//
//     }

 } else{
     return;
 }
}

void Cell::createDaughters() {
double x_half, y_half, z_half;
    x_half = (getXmin()+getXmax())/2.0;
    y_half = (getYmin()+getYmax())/2.0;
    z_half = (getZmin()+getZmax())/2.0;
    //There are 8 daughters
    Cell daughter_1 = Cell(getXmin(), x_half, getYmin(), y_half, getZmin(),z_half, getName()+".0");
    Cell daughter_2 = Cell(x_half, getXmax(), getYmin(), y_half, getZmin(),z_half, getName()+".1");
    Cell daughter_3 = Cell(getXmin(), x_half, y_half, getYmax(), getZmin(),z_half, getName()+".2");
    Cell daughter_4 = Cell(x_half, getXmax(), y_half, getYmax(), getZmin(),z_half, getName()+".3");
    Cell daughter_5 = Cell(getXmin(), x_half, getYmin(), y_half, z_half,getZmax(), getName()+".4");
    Cell daughter_6 = Cell(x_half, getXmax(), getYmin(), y_half, z_half,getZmax(), getName()+".5");
    Cell daughter_7 = Cell(getXmin(), x_half, y_half, getYmax(), z_half,getZmax(), getName()+".6");
    Cell daughter_8 = Cell(x_half, getXmax(), y_half, getYmax(), z_half,getZmax(), getName()+".7");
    daughters = {daughter_1, daughter_2, daughter_3, daughter_4, daughter_5, daughter_6, daughter_7, daughter_8};
//    std::cout<<"Cell "<< getName()<<"creates daughters "<< std::endl;
//    for (auto daughter: daughters) {
//        std::cout <<"Daughter: " << daughter.getName() <<" has  "<< daughter.getN()<< std::endl;
//    }
    Cell::setHasDaughters(true);
}

std::vector<Particle> Cell::particles() {
    if (isOccupied()){
        std::vector<Particle> particles = {particle};//{getParticle()};
        //std::cout <<"Cell: " << getName() <<" has  particle "<< particle.getName() << std::endl;
        return particles;
    }
    else if(isHasDaughters()){ //Forward request to daughters
        std::vector<Particle> particles;
        //std::vector<Cell> daughters = getDaughters();
        for (auto &daughter: daughters) {
            std::vector<Particle> daughterParticles = daughter.particles();
            particles.insert(particles.end(), daughterParticles.begin(), daughterParticles.end());
        }
        //std::cout <<"Cell: " << getName() <<" after inserting daughters  "<< particles.size() << std::endl;
        return particles;
    }
    else {
        return std::vector<Particle>();
    }
}

bool Cell::meetsCriterion(Particle pParticle, double tree_thres, double softening) {
    if(isHasDaughters()){
        double s, dx, dy, dz, d;
        s = getXmax() - getXmin();
        dx = particle.getX() - getXcen();
        dy = particle.getY() - getYcen();
        dz = particle.getZ() - getZcen();
        d = pow((dx*dx + dy*dy + dz*dz),0.5);
        return (((d/s)>tree_thres)&&(d>softening)); //Threshold for the use of the tree
    }
    else {
        //std::cout <<"Particle: " << particle.getName() <<" is the same as particle "<< pParticle.getName() <<" : " << (particle!=pParticle) << std::endl;
        return (particle!=pParticle);
    }
}






