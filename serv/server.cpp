#include "servH.h"
#include "../common/packages.h"
#include "../common/comon.h"
#include <sstream>

using namespace std;

//read package
void *Communicate(void *threadarg)
{
    struct thread_data *my_data;
    my_data = (struct thread_data *)threadarg;

    while (nameSet(my_data->clientSocket, my_data->id) == false)
        ;
    cout << "sikeres nev valasztas" << endl;

    while (true)
    {
        vector<char> buf(514);
        int res = recv(my_data->clientSocket, buf.data(), 514, 0);
        while (res < 514)
        {
            vector<char> bufextra(514);
            int extra = recv(my_data->clientSocket, bufextra.data(), 514 - res, 0);
            if (!resCheck(extra))
            {
                cout << "EXIT RES\n";
                threadExit(my_data->id, my_data->clientSocket);
            }
            res += extra;
            cout << "extra res: " << extra << endl;
            for (int i = 0; i < extra; i++)
            {
                buf.push_back(bufextra.at(i));
            }
        }
        cout << "Kezdo res: " << res << endl;
        if (!resCheck(res))
        {
            cout << "EXIT RES\n";
            threadExit(my_data->id, my_data->clientSocket);
        }
        if (!decodeRECV(buf, my_data->clientSocket, my_data->id))
        {
            cout << "EXIT RECV\n";
            threadExit(my_data->id, my_data->clientSocket);
        }

        cout << "\nREVC vege" << endl;
    }
    threadExit(my_data->id, my_data->clientSocket);
}

int main()
{
    int rc;
    atexit(kilep);
    //szalak tomb inicializalasa
    for (int i = 0; i < NUM_THREADS; i++)
    {
        szal[i] = 0;
    }

    semStart();
    startConn();

    cout << "The server waiting for clients" << endl;
    while (true)
    {
        int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);
        cout << "main() : creating thread, " << endl;
        //tomb modositasa
        if (semop(semid, &down, 1) < 0)
        {
            cout << "Sem down mod error" << endl;
            exit(-1);
        }
        int i = elsoUresSzal();
        if (i < 0)
        {
            cout << "nem lehet tobb szalat letrehozni,\n probalkozzon kesobb" << endl;
            continue;
        }
        td[i].clientSocket = clientSocket;
        td[i].id = i;
        szal[i] = 1;
        if (semop(semid, &up, 1) < 0)
        {
            cout << "Sem up mod error" << endl;
            exit(-1);
        }
        rc = pthread_create(&threads[i], NULL, Communicate, (void *)&td[i]);
        if (rc)
        {
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }

    pthread_exit(NULL);
    return 0;
}