#include "../common/packages.h"
#include "decode.h"
#include "../common/comon.h"

using namespace std;

void kilep(void)
{
    cout << "KILEPES\n";
    pthread_exit(&threads[0]);
    close(sock);
    exit(0);
}

void MakeSocket()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        cout << "socket error" << endl;
        exit(1);
    }

    //	Create a hint structure for the server we're connecting with
    int port = 54000;
    string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //	Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr *)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        cout << "connect error" << endl;
        exit(1);
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
        if (!sendPack(sock, Retpack))
        {
            exit(-1);
        }

        //var a szerver valaszara
        std::vector<char> buf(515);
        res = recv(sock, buf.data(), 515, 0);
        if (!resCheck(res))
        {
            exit(-1);
        }
        if (buf.at(0) == 'O' && buf.at(1) == 'K')
        {
            cout << "Sikeres Nev beiras" << endl;
            break;
        }
        else
        {
            cout << "A nev foglalt probaljon mas nevet" << endl;
        }
    }
}
