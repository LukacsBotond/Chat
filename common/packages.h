#ifndef packages_H
#define packages_H

#include <iostream>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <vector>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

inline unsigned char uresChar[4]={0};

struct AllPackage{
    char type=1;
    unsigned char sorszam[4]={0};
    char adat[512-1-4]={0};
};

//file es csoportra is jo, csak a type-t kell valtoztatni
struct PrivPackage
{
    char type;
    unsigned char sorszam[4]={0};
    char fogado[10]={0};
    char adat[512-1-4-10]={0};
};

struct RetRegPackage{
    char type;
    char adat[511]={0};//nev
};
//sorszam intet char tombbe alakitja, hogy at lehessen kuldeni
unsigned char* IntToChar(unsigned char ures[4],unsigned int sorszam);
//visszaalakitja a char tombot
unsigned int CharToInt(unsigned char CsorSz[4]);

//belepesre es szerver visszaigazolasra is jo
std::string RetRegPackageGEN(char type,std::string adat);

//all csomag generator
//megadjuk a stringet amit el akarunk kuldeni es a csomag szamat
//a csomag szama alapjan csinal egy csomagot egy resz
//stringgel, 
std::string AllPackageGEN(std::string data,unsigned int sorszam);

std::string PrivPackageGEN(char type,unsigned int sorszam,std::string fogado,std::string data);

//0-kat ir a string vegere, hogy meglegyen a csomag merete
std::string fillData(std::string adat,unsigned int length);



#endif