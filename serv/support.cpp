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
    
    case 2:
        cout<<"-Csoport uzenet erkezett\n";

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

    case 6:
        cout << "-create chatszoba parancs erkezett\n";
        CreateGroup(buf, sock);
        return true;
        break;
    case 7:
        cout<<"-join chatszoba parancs erkezett\n";
        JoinGroup(buf,sock);
        return true;
        break;

    default:
        cout << "Nincs ilyen parancs\n";
        return false;
        break;
    }
    return false;
}

void KovCsomag(std::list<std::vector<char>> &csomagok, vector<int> kliensek, unsigned int &utCsomag)
{
    unsigned int sorszam;
    bool talalt = true;
    while (talalt)
    {
        talalt = false;
        for (auto it : csomagok)
        {
            sorszam = getSorszam(it);
            if (sorszam == utCsomag)
            {
                talalt = true;
                string csomag(it.begin(), it.end());
                sendVector(kliensek, csomag, utCsomag);
                csomagok.remove(it);
                utCsomag++;
                break;
            }
        }
    }
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
        }
        kliensek.push_back(td[x].clientSocket);
        break;
    }
    default:
    {
        cout << "ismeretlen parancs\n";
        break;
    }
    }
    return kliensek;
}

std::string getNev(std::string namebad)
{
    string cimzett = "";
    for (int i = 0; i < 11; ++i)
    {
        if (namebad[i] == '\0')
            break;
        cimzett += namebad[i];
    }
    return cimzett;
}
