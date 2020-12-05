#include "../common/packages.h"
#include "decode.h"
#include "../common/comon.h"

using namespace std;

void kilep(void)
{
    cout << "KILEPES\n";
    pthread_exit(&threads[0]);
    close(sock);
}

void semStart()
{
    ////////////////////////// SZEMAFOR /////////////////////
    if ((semid = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) < 0)
    {
        cout << "semget error" << endl;
        exit(-1);
    }

    short init[1] = {1};
    if (semctl(semid, 0, SETALL, init) < 0)
    {
        cout << "INIT\n";
        exit(-1);
    }
}

void sendName()
{
    int res;
    while (true)
    {
        string name;
        string Retpack = "";
        cout << "Irjon egy felhasznalo nevet: ";
        getline(cin, name);

        if (name.length() > 10)
        {
            cout << "A nev tul hosszu, max 10 karakter!\n";
            continue;
        }
        try
        {
            Retpack = RetRegPackageGEN(4, name);
        }
        catch (length_error &e)
        {
            cout << e.what() << '\n';
            continue;
        }

        //Send to server
        if(!sendPack(Retpack)){
            exit(-1);
        }

        std::vector<char> buf(515);
        res = recv(sock, buf.data(), 515, 0);
        if (!resCheck(res))
        {
            exit(-1);
        }
        cout << buf.at(0) << buf.at(1);
        if (buf.at(0) == 'O' && buf.at(1) == 'K')
        {
            cout << "sikeres Nev beiras" << endl;
            break;
        }
        else
        {
            cout << "A nev foglalt probaljon mas nevet" << endl;
        }
    }
}
