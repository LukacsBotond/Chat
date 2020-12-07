#include "decode.h"
#include "../common/packages.h"
#include "../common/comon.h"

using namespace std;

void *recv(void *threadarg)
{
    struct thread_data *my_data;
    my_data = (struct thread_data *)threadarg;
    vector<char> buf(515);
    while (true)
    {
        int bytesReceived = recv(my_data->sock, buf.data(), 515, 0);
        cout<<"bytes: "<<bytesReceived<<endl;
        if(!resCheck(bytesReceived)){
            cout<<"bytes: "<<bytesReceived<<endl;
            vege = true;
            pthread_exit(NULL);
        }
        decodeRevc(buf);
    }
}

int main()
{
    /*
    string data = "ALMAFA DE NAGY";
    string test;
    try
    {
        test = AllPackageGEN(data,UINT32_MAX);
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
*/
    
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
    cout<<"Lehetseges parancsok:\n";
    cout<<"-all uzenet vagy -a uzenet   Mindenki aki csatlakozva van kuld egy uzenetet\n";
    cout<<"-list vagy -l Visszateriti az aktiv felhasznalokat\n";
    cout<<"-priv cimzett uzenet vagy -p cimzett uzenet Privat uzenet kuldese egy felhasznalonak";
    cout<<"-file fajlnev vagy -f fajlnev\n -all mindenkinek \n-p felhasznalonev felhasznalonak \n-g csoportnev csoportnak\n\n\n";

    do
    { //		Enter lines of text
        //cout << "PARANCS>: ";
        getline(cin, userInput);
        if(!decodeCommand()){
            cout<<"Ismeretlen parancs, probalja ujra"<<endl;
        }
    } while (!vege);
    return 0;
}