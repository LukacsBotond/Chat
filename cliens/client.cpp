#include "decode.h"
#include "../common/packages.h"

using namespace std;

void kilep(void)
{
    cout << "KILEPES\n";
    pthread_exit(&threads[0]);
    close(sock);
}

void *recv(void *threadarg)
{
    struct thread_data *my_data;
    my_data = (struct thread_data *)threadarg;
    char buf[8000];
    memset(buf, 0, 8000);
    while (true)
    {
        int bytesReceived = recv(my_data->sock, buf, 8000, 0);
        if (bytesReceived == -1)
        {
            cout << "There was an error getting response from server\r\n";
        }
        if (bytesReceived == 0)
        {
            cout << "Server disconnected\n";
            exit(0);
        }
        else
        {
            cout << "SERVER> " << string(buf, bytesReceived) << "\r\n";
        }
    }
    close(my_data->sock);

    pthread_exit(NULL);
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
        res = send(sock, Retpack.c_str(), Retpack.length() + 1, 0);
        if (res == -1)
        {
            cout << "nem sikerult a szerverhez kuldeni!\r\n";
            exit(-1);
        }

        std::vector<char> buf(515);
        res = recv(sock, buf.data(), 515, 0);
        if (res == -1)
        {
            cout << "There was an error getting response from server\r\n";
        }
        if (res == 0)
        {
            cout << "Server disconnected\n";
            exit(0);
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

int main()
{
    
    string data = "ALMAFA DE NAGY";
    string test;
    try
    {
        test = AllPackageGEN(data, 0);
        cout << "END" << endl;
    }
    catch (out_of_range &e)
    {
        std::cout<< e.what() << '\n';
    }
    catch (length_error &e)
    {
        std::cout << e.what() << '\n';
    }

    try
    {
        test = PrivPackageGEN('0', 0, "BOB", data);
        cout << "END" << endl;
    }
    catch (out_of_range &e)
    {
        std::cout << e.what() << '\n';
    }
    catch (length_error &e)
    {
        std::cout << e.what() << '\n';
    }

    /*
    atexit(kilep);
    pthread_t threads[1];
    struct thread_data td[1];
    //	Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        cout << "socket error" << endl;
        return 1;
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
        return 1;
    }

    //send Username to server, wait for it to say it is fine/not fine
    //if it is fine then proceed
    sendName();

    ///recv resze
    /*
    td[0].sock = sock;
    int rc = pthread_create(&threads[0], NULL, recv, (void *)&td[0]);
    if (rc)
    {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }
    */
    //	While loop:

    /*
    //cliens write the command
    cout<<"Lehetseges parancsok:\n";
    cout<<"-all uzenet vagy -a uzenet   Mindenki aki csatlakozva van kuld egy uzenetet\n";
    cout<<"-list vagy -l Visszateriti az aktiv felhasznalokat\n";
    cout<<"-priv cimzett uzenet vagy -p cimzett uzenet Privat uzenet kuldese egy felhasznalonak";
    cout<<"-file fajlnev vagy -f fajlnev\n -all mindenkinek \n-p felhasznalonev felhasznalonak \n-g csoportnev csoportnak\n";

    string userInput;
    do
    { //		Enter lines of text
        cout << "> ";
        getline(cin, userInput);
        if(!decodeCommand(userInput)){
            cout<<"Hiba tortent, probalja ujra"<<endl;
        }
    } while (true);
    */
    return 0;
}