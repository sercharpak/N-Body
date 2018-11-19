//
// Created by shernand on 07/06/18.
//

#ifndef PROJECT_WRITTER_H
#define PROJECT_WRITTER_H


#include <string>
#include <vector>
#include "Particle.h"

class Writter {
public:
    /**
     * Writes the positions of the particles in a text file
     * in format x y z
     * @param fileName
     */
    static void writePositionsText(std::string fileName, int id, std::vector<Particle> particles);

};


#endif //PROJECT_WRITTER_H
