#include "comon.h"
#include "packages.h"
#include <iostream>
#include <vector>

using namespace std;

bool resCheck(int res)
{
    if (res < 0)
    {
        cout << "RECV error, exit thread\n";
        return false;
    }
    if (res == 0)
    {
        cout << "kliens kilepett\n";
        return false;
    }
    return true;
}


unsigned int getSorszam(std::vector<char> buf)
{
    unsigned char Csorsz[4];
    Csorsz[0] = buf.at(1);
    Csorsz[1] = buf.at(2);
    Csorsz[2] = buf.at(3);
    Csorsz[3] = buf.at(4);
    unsigned int sorszam = CharToInt(Csorsz);
    return sorszam;
}
