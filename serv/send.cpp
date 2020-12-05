#include "servH.h"
#include "../common/packages.h"
#include "../common/comon.h"

using namespace std;

void sendAll(char *data)
{
    if (semop(semid, &down, 1) < 0)
    {
        cout << "Sem down error" << endl;
        exit(-1);
    }
    /*
        int semval = semctl(semid, 0, GETVAL, 1);
        cout << "SEMVAL: " << semval << endl;
        cout << "ENTER critical section\n";
        */
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (!szal[i])
        {
            continue;
        }
        int bytessend = send(td[i].clientSocket, data, strlen(data) + 1, 0);
        if (bytessend == -1)
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

bool sendValsz(int sock, string data)
{
    string Retpack = "";
    try
    {
        Retpack = RetRegPackageGEN(10,data);
    }
    catch (length_error &e)
    {
        cout << e.what() << '\n';
    }

    int res = send(sock, Retpack.c_str(), Retpack.length() + 1, 0);
    cout << "res: " << res << endl;
    if (!resCheck(res))
    {
        return false;
    }
    return true;
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
    int bytessend = send(sock, RetNev.c_str(), RetNev.length() + 1, 0);
    if (bytessend == -1)
    {
        cout << "hiba a kuldes soran\n";
    }

    if (semop(semid, &up, 1) < 0)
    {
        cout << "Sem down error" << endl;
        return;
    }
}


