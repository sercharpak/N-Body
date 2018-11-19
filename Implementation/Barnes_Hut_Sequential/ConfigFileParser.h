//
// Created by sergi on 6/6/2018.
//
/** @file ConfigFileParser.h
 *  @author Sergio Hernandez
 *
 * This file is part of the N-Body Project for the 2018 PHPC Course.
 *
 *
 * This class performs the parsing of a standard configuration file.
 *
 */
#ifndef PROJECT_CONFIGFILEPARSER_H
#define PROJECT_CONFIGFILEPARSER_H


#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "FileNotFoundException.h"
#include "FileParserException.h"

/** Class to parse a configuration file following the standard specified in the documentation.
 */
class ConfigFileParser {
private:
    /**
     * The default keys that should be in the config file
     */
    std::vector<std::string> defaultKeys;
    /**
     * The default values for the i/o operations
     */
    std::vector<std::string> defaultIO;
    /**
     * The default values for the type of filters
     */
    std::vector<std::string> defaultFilters;
    /**
     * The filename (path) of the configuration file
     */
    std::string fName;
    /**
     * The map of the keys and values specified in the configuration file
     * The filters are parsed later on and specified in a vector
     */
    std::map<std::string,std::string> data;
public:
    /** Standard Constructor
     */
    ConfigFileParser();
    /** Standard Destructor
     */
    ~ConfigFileParser();
    /** Gives the map of the keys and values in the configuration file
     * @return std::map<std::string,std::string> The map of the configuration file
     */
    std::map<std::string,std::string> getData();
    /** Sets the map of the keys and values in the configuration file
     * @param std::map<std::string,std::string> pData The map of the configuration file
     */
    void setData(std::map<std::string,std::string> pData);
    /** Gives the name (path) of the configfile
     * @return std::string the name (path) of the configfile
     */
    std::string getFName();

    /**
     * Parses the file and stores the resulting map and filters vector
     * @param pFname The filename (path) of the configuration file
     */
    void parseFile(std::string pFname);

    /**
     * Verifies that the parsed file is well formatted and formed
     */
    void verify();

    /**
     * Verifies a value exists for a key in the map data
     * @param std::string the key for which we want to verify the existence of the value
     */
    bool valueExistsForKey(std::string key);


};


#endif //PROJECT_CONFIGFILEPARSER_H
