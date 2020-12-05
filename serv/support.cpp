#include "servH.h"
#include "../common/packages.h"
#include "../common/comon.h"

using namespace std;


bool correctPack(unsigned int sorszam, unsigned int csomagszamlal)
{
    //az a csomag erkezett amire vartunk
    if (sorszam == csomagszamlal)
    {
        return true;
    }
    return false;
}

bool valasz(int sock, unsigned int &sorszam, unsigned int &csomagszamlal)
{
    if (sorszam == UINT32_MAX)
    { //utolso csomag a kuldotol
        csomagszamlal = 0;
        return sendValsz(sock, "OK");
    }
    else
    //varakozas a kommunikacio kovetkezo csomagjara
    {
        if (correctPack(sorszam, csomagszamlal))
        {
            return sendValsz(sock, "OK");
            csomagszamlal++;
        }
        else
        {
            //ha nem jo akkor visszakuldi az utolso jo csomag
            //sorszamat
            unsigned char* sorsz = IntToChar(uresChar,csomagszamlal);
            string val(reinterpret_cast<char*>(sorsz));
            val=val.substr(0,4);
            return sendValsz(sock,val);
        }
    }
}
