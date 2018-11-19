//
// Created by sergi on 6/6/2018.
//
/** @file ConfigFileParser.cpp
 *  @author Sergio Hernandez
 *
 * This file is part of the N-Body Project for the 2018 PHPC Course.
 *
 *
 * This class performs the parsing of a standard configuration file.
 *
 */
#include "ConfigFileParser.h"

ConfigFileParser::ConfigFileParser() {
        defaultKeys.push_back("type_input");
        defaultKeys.push_back("newton_g");
        defaultKeys.push_back("timestep");
        defaultKeys.push_back("box_size");
        defaultKeys.push_back("softening");
        defaultKeys.push_back("tree_thres");
        defaultKeys.push_back("display_f");
        defaultKeys.push_back("n_particles");
        defaultKeys.push_back("scale_radius");
        defaultKeys.push_back("n_time_steps");
        defaultKeys.push_back("type_output");
        defaultKeys.push_back("outputFile");
        defaultKeys.push_back("number_outputs");
}
ConfigFileParser::~ConfigFileParser(){}

std::map<std::string,std::string> ConfigFileParser::getData(){
    return data;
}

void ConfigFileParser::setData(std::map<std::string,std::string> pData)
{
    data=pData;
}
std::string ConfigFileParser::getFName(){
    return fName;
}

void ConfigFileParser::parseFile(std::string pFname){
    fName = pFname;
    std::cout<<"Parsing file: " << fName << std::endl;
    std::ifstream file;
    file.open(fName.c_str());
    if (!file)
        throw FileNotFoundException();
    std::string line;
    while (std::getline(file, line))
    {
        std::string temp = line;
        if (temp.empty()) //Empty Line
            continue;
        if(temp.front() =='#') //Comment Line
            continue;

        std::istringstream str_line(temp);
        std::string key;
        if( std::getline(str_line, key, '=') )
        {
            std::string value;

            if( std::getline(str_line, value) ){
                if (!key.empty() && !value.empty()) { //Verify that the two values exist
                    data[key] = value;
                    //std::cout << "key: " << key << std::endl;
                    //std::cout << "value: " << value << std::endl;
                }
                else
                    throw FileParserException();
            }
        }
    }
    file.close();



}
void ConfigFileParser::verify(){
    for(int i=0;i<defaultKeys.size();++i){
        std::string defaultKey = defaultKeys[i];
        valueExistsForKey(defaultKey);
    }

}
bool ConfigFileParser::valueExistsForKey(std::string key){
    auto iter = data.find(key);
    if (iter == data.end())
        throw FileParserException();
    else
        return true;
}
