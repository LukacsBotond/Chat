#include "servH.h"
#include "../common/packages.h"
#include "../common/comon.h"

using namespace std;

bool decodeRECV(std::vector<char> buf, int sock, int id)
{
    string csomag(buf.begin(), buf.end());
    //milyen tipus erkezik
    switch (buf.at(0))
    {

    case 0: //privat uzenet
        cout << "privat uzenet" << endl;
        SendPriv(buf, sock, id);
        return true;
        break;

    case 1: //mindenkinek kuldes
        cout << "-ALL parancs erkezett" << endl;
        sendAll(csomag);
        return true;
        break;

    case 3:
        cout << "File Send\n";
        SendFile(buf, sock);
        return true;
        break;
    case 5:
        cout << "-list parancs erkezett\n";
        SendNames(sock);
        return true;
        break;

    default:
        cout << "Nincs ilyen parancs\n";
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

std::vector<char> KovCsomag(std::list<std::vector<char>> csomagok, int utCsomag)
{
}

std::vector<int> FindClinets(std::string parancs, std::string cimzett)
{
    vector<int> kliensek;

    switch (parancs[1])
    {
    case 'a':
    {
        for (int i = 0; i < NUM_THREADS; i++)
        {
            if (!szal[i])
                continue;
            kliensek.push_back(td[i].clientSocket);
        }
        break;
    }
    case 'p':
    {
        auto it = find(begin(nevek), end(nevek), cimzett);
        int x = it - begin(nevek);
        if (x >= NUM_THREADS)
        {
            cout << "Nincs ilyen felhasznalo" << endl;
            throw out_of_range("");
        }
        kliensek.push_back(td[x].clientSocket);
        break;
    }
    default:
    {
        cout << "ismeretlen parancs\n";
        throw out_of_range("");
        break;
    }
    }
    return kliensek;
}
