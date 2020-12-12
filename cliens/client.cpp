#include "decode.h"
#include "../common/packages.h"
#include "../common/comon.h"

using namespace std;

void *recv(void *threadarg)
{
    struct thread_data *my_data;
    my_data = (struct thread_data *)threadarg;
    vector<char> buf(514);
    while (true)
    {
        int res = recv(my_data->sock, buf.data(), 514, 0);
        while (res < 514)
        {
            vector<char> bufextra(514);
            int extra = recv(sock, bufextra.data(), 514 - res, 0);
            //cout << "extra res: " << extra << endl;
            for (int i = 0; i < extra; i++)
            {
                buf.at(i + res) = bufextra.at(i);
            }
            res += extra;
        }
        if (!resCheck(res))
        {
            cout << "bytes: " << res << endl;
            vege = true;
            pthread_exit(NULL);
        }
        decodeRevc(buf);
    }
}

int main()
{
    atexit(kilep);
    pthread_t threads[1];
    struct thread_data td[1];
    //	Create a socket

    MakeSocket();

    //send Username to server, wait for it to say it is fine/not fine
    //if it is fine then proceed
    sendName();

    ///recv resze

    td[0].sock = sock;
    int rc = pthread_create(&threads[0], NULL, recv, (void *)&td[0]);
    if (rc)
    {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }

    //	While loop:
    //cliens write the command
    cout << "Lehetseges parancsok:\n";
    cout << "-all uzenet vagy -a uzenet :Mindenki aki csatlakozva van kuld egy uzenetet\n";
    cout << "-list vagy -l Visszateriti az aktiv felhasznalokat\n";
    cout << "-priv cimzett uzenet vagy -p cimzett uzenet :Privat uzenet kuldese egy felhasznalonak\n";
    cout << "-file fajlnev vagy -f fajlnev\n\t-a mindenkinek \n\t-p felhasznalonev felhasznalonak \n\t-g csoportnev csoportnak\n";
    cout << "-c chatszobanev: Chatszoba letrehozasa\n";
    cout << "-j chatszobaNev :-chatszobaba valo belepes\n";
    cout << "-g chatszobaNev uzenet: csoportnak az uzenet elkuldese\n";
    do
    { //		Enter lines of text
        cout << "PARANCS>: ";
        getline(cin, userInput);
        if (!decodeCommand())
        {
            cout << "Ismeretlen parancs, probalja ujra" << endl;
        }
        else
        {
            cout << "Sikeresen elkuldve\n";
        }
    } while (!vege);
    return 0;
}