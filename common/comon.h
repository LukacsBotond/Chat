#ifndef comon_H
#define comon_H

#include <vector>
#include <iostream>
#include <sstream>
#include <list>

bool resCheck(int res);

unsigned int getSorszam(std::vector<char> buf);

bool sendPack(int socket, std::string pack);

bool correctPack(unsigned int sorszam,unsigned int csomagszamlal);


#endif
