#include <iostream>
#include <string.h>
#include "packages.h"

using namespace std;

unsigned char *IntToChar(unsigned char ures[4], unsigned int sorszam)
{
    for (int i = 3; i >= 0; i--)
    {
        ures[i] = sorszam % 256;
        sorszam /= 256;
    }
    return ures;
}

unsigned int CharToInt(unsigned char *CsorSz)
{
    int sorszam = 0;
    int mult = 1;
    for (int i = 3; i >= 0; i--)
    {
        sorszam += ((unsigned int)CsorSz[i] * mult);
        mult *= 255;
    }
    return sorszam;
}

std::string fillData(string adat,unsigned int length)
{
    unsigned int adatLen = adat.length();
    for (unsigned int i = adatLen; i < length; i++)
    {
        adat += (char)0;
    }
    return adat;
}

string RetRegPackageGEN(char type, std::string adat)
{
    if (adat.length() == 0)
    {
        throw length_error("Nincs string");
    }
    string csomag="";
    csomag+=(type-'0');
    csomag += fillData(adat, 511);
    return csomag;
}

string AllPackageGEN(string data, unsigned int sorszam)
{
    if (data.length() == 0)
    {
        throw length_error("Nincs string");
    }

    unsigned int kezdo = sorszam * 507;

    if(sorszam == UINT32_MAX){
        int l = data.length();
        int parts=l/507;
        kezdo = (507*parts);
    }
    if (kezdo >= data.length())
    {
        throw out_of_range("tul nagy a csomag indexe");
    }
    string csomag = "1";//csomag tipusa
    unsigned char* sorsz=IntToChar(uresChar, sorszam);
    //copy sorszam
    for(int i=3;i>=0;--i){
        csomag+=sorsz[i];
    }
    csomag[0]=1;
    
    csomag += fillData(data.substr(kezdo, 507),507);
    cout << "csomag: "<<csomag << endl;
    return csomag;
}

string PrivPackageGEN(char type,unsigned int sorszam,string fogado,std::string data){
    if (data.length() == 0)
    {
        throw length_error("Nincs string");
    }
    if (fogado.length() == 0)
    {
        throw length_error("Nincs cimzett");
    }
    unsigned int kezdo = sorszam * 497;
    if (kezdo >= data.length())
    {
        throw out_of_range("A string nem eleg hosszu");
    }
    string csomag="";
    csomag+=type-'0';
    //copy sorszam
    unsigned char* sorsz=IntToChar(uresChar, sorszam);
    for(int i=3;i>=0;--i){
        csomag+=sorsz[i];
    }
    csomag += fillData(fogado,10);
    csomag += fillData(data.substr(kezdo, 497),497);
    return csomag;
}
