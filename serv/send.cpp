#include "servH.h"
#include "../common/packages.h"
#include "../common/comon.h"

using namespace std;

void sendAll(string csomag)
{
    if (semop(semid, &down, 1) < 0)
    {
        cout << "Sem down error" << endl;
        exit(-1);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (!szal[i])
        {
            continue;
        }
        //int bytessend = send(td[i].clientSocket, csomag.c_str(), 513, 0);
        if (!sendPack(td[i].clientSocket, csomag))
        {
            cout << "hiba a kuldes soran\n";
        }
    }
    if (semop(semid, &up, 1) < 0)
    {
        cout << "Sem up error" << endl;
        exit(-1);
    }
}

void SendNames(int sock)
{
    if (semop(semid, &down, 1) < 0)
    {
        cout << "Sem down error" << endl;
        return;
    }
    string RetNev = "A szerveren aktiv felhasznalok: ";
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (nevek[i] != "")
        {
            RetNev += (nevek[i] + ' ');
        }
    }
    int kezdo = 0;
    string returnPack;
    unsigned int osszcsomag = (RetNev.length() / 511) + 1;
    for (unsigned int i = 0; i < osszcsomag; i++)
    {
        returnPack = RetRegPackageGEN('5', RetNev.substr(kezdo, 511));
        if (!sendPack(sock, returnPack))
        {
            cout << "HIBA NEV kuldeskor\n";
            return;
        }
    }
    if (semop(semid, &up, 1) < 0)
    {
        cout << "Sem up error" << endl;
        return;
    }
}

void SendPriv(vector<char> buf,int sock,int id)
{
    string csomag(buf.begin(), buf.end());
    string cimzett = "";
    for (int i = 5; i < 16; ++i)
    {
        if (buf.at(i) == '\0')
            break;
        cimzett += buf[i];
    }
    cout << "Cimzett: " << cimzett << endl;
    if (semop(semid, &down, 1) < 0)
    {
        cout << "Sem down error" << endl;
        return;
    }
    string RetPack;

    auto it = find(begin(nevek), end(nevek), cimzett);
    int x = it - begin(nevek);
    if (x < NUM_THREADS)
    {
        cout << "Van ilyen felhasznalo" << endl;
        //RetPack = fillData("NO", 512);
    }
    else
    { //nincs benne a nev
        cout << "Nincs ilyen felhasznalo" << endl;
        RetPack = RetRegPackageGEN('9'+1, "Error, nincs ilyen aktiv felhasznalo");
        if (!sendPack(sock, RetPack))
        {
            cout << "hiba a kuldes soran\n";
        }
        return;
    }

    //nevev kicserelese a cimzettrol a feladora
    for(int i=5;i<15;i++){
        csomag[i]=nevek[id][i-5];
    }
    csomag[15]=':';

    if (!sendPack(td[x].clientSocket, csomag))
    {
        cout << "hiba a kuldes soran\n";
    }

    if (semop(semid, &up, 1) < 0)
    {
        cout << "Sem up error" << endl;
        return;
    }
}
