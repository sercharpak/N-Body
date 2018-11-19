//
// Created by sergi on 6/6/2018.
//
/** @file FileParserException.hpp
 *  @author Sergio Hernandez
 *
 * This file is part of the N-Body Project for the 2018 PHPC Course.
 *
 *
 * This class defines an Exception in the case of the occurrance of an error during the parsing
 * of the configuration file.
 *
 */
#ifndef PROJECT_FILEPARSEREXCEPTION_H
#define PROJECT_FILEPARSEREXCEPTION_H
#include <string>
#include <stdexcept>
class FileParserException : public std::runtime_error{
public:
    FileParserException() : std::runtime_error("FILE PARSER ERROR: The parsing of the file failed. \n Please verify the file is well formatted.") { }
    virtual const char* what() const throw()
    {
        return "FILE PARSER ERROR: The parsing of the file failed. \n Please verify the file is well formatted.";
    }

};
#endif //PROJECT_FILEPARSEREXCEPTION_H
