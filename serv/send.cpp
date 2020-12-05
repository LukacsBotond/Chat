#include "servH.h"
#include "../common/packages.h"

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