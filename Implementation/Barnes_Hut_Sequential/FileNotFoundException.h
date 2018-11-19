//
// Created by sergi on 6/6/2018.
//
/** @file FileNotFoundException.h
 *  @author Sergio Hernandez
 *
 * This file is part of the N-Body Project for the 2018 PHPC Course.
 *
 *
 * This class defines an Exception in the case of a file not found required to read.
 *
 */
#ifndef PROJECT_FILENOTFOUNDEXCEPTION_H
#define PROJECT_FILENOTFOUNDEXCEPTION_H
#include <string>
#include <stdexcept>
class FileNotFoundException : public std::runtime_error{
public:
    FileNotFoundException() : std::runtime_error("FILE NOT FOUND: The file cannot be found") { }
    virtual const char* what() const throw()
    {
        return "FILE NOT FOUND: The file cannot be found";
    }

};
#endif //PROJECT_FILENOTFOUNDEXCEPTION_H
