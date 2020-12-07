#include "servH.h"
#include "../common/packages.h"
#include "../common/comon.h"

using namespace std;

bool decodeRECV(std::vector<char> buf,int sock)
{
    unsigned int sorszam = getSorszam(buf);
    string csomag(buf.begin(), buf.end());
    //milyen tipus erkezik
    switch (buf.at(0))
    {
    //mindenkinek kuldes
    case 1:
        cout << "-ALL parancs erkezett" << endl;
        sendAll(csomag);
        return true;
        break;
    case 5:
        cout<<"-list parancs erkezett\n";
        SendNames(sock);
        return true;
        break;

    default:
        cout<<"Nincs ilyen parancs\n";
        return false;
        break;
    }
    return false;
}

bool correctPack(unsigned int sorszam, unsigned int csomagszamlal)
{
    //az a csomag erkezett amire vartunk
    if (sorszam == csomagszamlal)
    {
        return true;
    }
    return false;
}
