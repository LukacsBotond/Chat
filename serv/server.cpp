#include "servH.h"
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
        std::vector<char> buf(515);

        int res = recv(my_data->clientSocket, buf.data(), 515, 0);
        cout<<"res: "<<res<<endl;
        if (res < 0)
        {
            if (errno == EINTR)
            {
                // Interrupted, just try again ...
                continue;
            }
            else
            {
                cout << "RECV error, exit thread\n";
                threadExit(my_data->id, my_data->clientSocket);
            }
        }
        if (res == 0)
        {
            cout << "kliens kilepett\n";
            threadExit(my_data->id, my_data->clientSocket);
        }

        vector<int> adat;
        //milyen tipus erkezik
        switch (buf.at(0))
        {
        //mindenkinek kuldes
        case 1:
            cout << "-ALL parancs erkezett" << endl;
            adat = {buf.begin() + 5, buf.end()};
            break;

        default:
            threadExit(my_data->id, my_data->clientSocket);
            //throw runtime_error("Not implemented function");
            break;
        }

        cout<<"beerkezo adat kiirasa\n";
        for (unsigned int i = 0; i < adat.size(); i++)
        {
            cout <<(char) adat.at(i);
        }
        cout << "\nREVC vege" << endl;
    }
    /*
    //amig 
    while (nameSet(my_data->clientSocket, my_data->id) == false);
    cout << "Succesfull name set" << endl;

    string command;

    printNevek();

    while (true)
    {
        int offset = 0;
        std::vector<char> buf(512);
        bool first = true;
        char test[513];
        while (true)
        {
            int res = recv(my_data->clientSocket,test,512, 0);
            //int res = recv(my_data->clientSocket, buf.data() + offset, buf.size() - offset, 0);
            if (res < 0)
            {
                if (errno == EINTR)
                {
                    // Interrupted, just try again ...
                    continue;
                }
                else
                {
                    cout << "RECV error, exit thread\n";
                    threadExit(my_data->id, my_data->clientSocket);
                }
            }
            if (res == 0)
            {
                if (offset == 0)
                {
                    // Client did just close the connection
                    cout << "Cliens disconnected, exit thread\n";
                    threadExit(my_data->id, my_data->clientSocket);
                }
                else
                {
                    cout << "RECV error unexpected end, exit thread\n";
                    threadExit(my_data->id, my_data->clientSocket);
                }
            }
            //megkeresi a protokol parametereket
            if(first){
                first = false;
                cout<<"ELSO resz: "<<endl<<endl;
            }
            test[res]='\0';
            cout<<test;
            if (res == 512) //van meg
            {
                cout<<"van meg "<<offset<<endl;
                offset += res;
                
                
                buf.resize(buf.size()+res); // double available memory
                
               
               cout<<test;
            }
            else //vege
            {   
                offset += res;
                cout<<"vege "<<offset<<endl;
                offset += res;
                for (int i = 0; i < buf.size(); i++)
                {
                    cout << buf.at(i);
                }
                cout << endl;
                break;
            }
        }
        
        if (strcmp(buf, "/list") == 0)
        {
            SendNames(my_data->clientSocket);
        }
        if (strncmp(buf, "/all", 4) == 0)
        {
            sendAll(buf);
        }
        //szerver kiir mindent amit kapott
        
        cout << string(buf, 0, res) << endl;
        
    }
    */
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