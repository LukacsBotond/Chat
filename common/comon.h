#ifndef comon_H
#define comon_H

#include <vector>
#include <iostream>

bool resCheck(int res);

unsigned int getSorszam(std::vector<char> buf);

bool sendPack(int socket,std::string pack);

#endif
