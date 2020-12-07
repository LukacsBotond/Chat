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
    //reset, mivel vegeztunk a tomb olvasasaval
    cout << "LEAVE critical section\n";
    if (semop(semid, &up, 1) < 0)
    {
        cout << "Sem up error" << endl;
        exit(-1);
    }
}

//repair
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
    for(unsigned int i=0;i<osszcsomag;i++){
        returnPack=RetRegPackageGEN('5',RetNev.substr(kezdo,511));
        if(!sendPack(sock,returnPack)){
            cout<<"HIBA NEV kuldeskor\n";
            return;
        }
    }
    if (semop(semid, &up, 1) < 0)
    {
        cout << "Sem up error" << endl;
        return;
    }
}
