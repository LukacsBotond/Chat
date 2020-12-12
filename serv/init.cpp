#include "servH.h"

using namespace std;

bool nameSet(int clientSocket, int id)
{
    cout << "NAMESET start" << endl;
    vector<char> buf(514);
    int res;
    //read nameset package
    res = recv(clientSocket, buf.data(), 550, 0);
    if (!resCheck(res))
    {
        threadExit(id, clientSocket);
    }
    while (res < 514)
    {
        vector<char> bufextra(514);
        int extra = recv(clientSocket, bufextra.data(), 514 - res, 0);
        if (!resCheck(res))
        {
            cout << "EXIT RES\n";
            threadExit(id, clientSocket);
        }
        for (int i = 0; i < extra; i++)
            buf.at(i + res) = bufextra.at(i);
        res += extra;
    }
    //nev kivevese
    string badname(buf.begin() + 1, buf.begin() + 11);
    string nev = getNev(badname);
    ///////////////////////////////////////////////////////////
    semdown();
    string RetPack = "";
    RetPack += '4' - '0';
    auto it = find(begin(nevek), end(nevek), nev);
    if (it != end(nevek))
    { //benne van mar a nev
        RetPack = fillData("NO", 512);
    }
    else
    { //nincs benne a nev
        RetPack = fillData("OK", 512);
        nevek[id] = nev;
    }
    semup();
    ////////////////////////////////////////////////////////////

    if (!sendPack(clientSocket, RetPack))
    {
        cout << "hiba a kuldes soran\n";
        return false;
    }
    else
    {
        if (RetPack[0] == 'O' && RetPack[1] == 'K')
        {
            cout << "sikeres nev beiras" << endl;
            return true;
        }
        else
        {
            cout << "sikertelen nev beiras" << endl;
            return false;
        }
    }
}

void CreateGroup(std::vector<char> buf, int sock)
{
    string badName(buf.begin() + 1, buf.begin() + 11);
    string csopNev = getNev(badName);
    string pack = "";
    semdown();
    auto it = csoport.find(csopNev);
    if (it == csoport.end())
    { //nincs ilyen csoport
        pack = RetRegPackageGEN('9', "Sikeres Csoport letrehozas");
        list<int> csoportLista;
        csoportLista.push_back(sock);
        csoport.insert(pair<string, list<int>>(csopNev, csoportLista));
    }
    else
    {
        pack = RetRegPackageGEN('9', "CsoportNev mar foglalt");
    }

    if (!sendPack(sock, pack))
    {
        cout << "Sikertelen kuldes" << endl;
    }
    semup();
}

void JoinGroup(std::vector<char> buf, int sock)
{
    string badName(buf.begin() + 1, buf.begin() + 11);
    string csopNev = getNev(badName);
    string pack = "";
    semdown();
    auto it = csoport.find(csopNev);
    if (it == csoport.end())
    { //nincs ilyen csoport
        pack = RetRegPackageGEN('9', "Nincs ilyen nevu csoport");
    }
    else
    {
        it->second.push_back(sock);
        pack = RetRegPackageGEN('9', "Sikeres Csatlakozas");
    }

    if (!sendPack(sock, pack))
    {
        cout << "Sikertelen kuldes" << endl;
    }
    semup();
}

int elsoUresSzal()
{
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (szal[i] == 0)
        {
            return i;
        }
    }
    return -1;
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

void semup()
{
    if (semop(semid, &up, 1) < 0)
    {
        cout << "Sem up error" << endl;
        throw out_of_range("");
    }
}

void semdown()
{
    if (semop(semid, &down, 1) < 0)
    {
        cout << "Sem down error" << endl;
        throw out_of_range("");
    }
}

void startConn()
{
    // Create a socket
    listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cout << "Can't create a socket! Quitting" << endl;
        exit(1);
    }
    int opt = 1;

    if (setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        cout << "Failed to set SO_REUSEADDR option. " << strerror(errno) << "\n";
        exit(1);
    }

    if (setsockopt(listening, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        cout << "Failed to set SO_REUSEPORT option. " << strerror(errno) << "\n";
        exit(1);
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint = {0};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    if ((bind(listening, (sockaddr *)&hint, sizeof(hint))) == -1)
    {
        cout << "bind failed\n";
        exit(-1);
    }
    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);
}

void printNevek()
{
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (nevek[i] != "")
        {
            cout << nevek[i] << " ";
        }
    }
    cout << endl;
}

void threadExit(int id, int sock)
{
    // Close the socket
    close(sock);
    cout << "SZAL lezarasa\n";
    if (semop(semid, &down, 1) < 0)
    {
        cout << "Sem down error" << endl;
        exit(-1);
    }
    szal[id] = 0;
    nevek[id] = "";
    if (semop(semid, &up, 1) < 0)
    {
        cout << "Sem up error" << endl;
        exit(-1);
    }
    pthread_exit(NULL);
}

void kilep(void)
{
    semctl(semid, 0, IPC_RMID, 0); //szemafor tomb torlese
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (szal[i] != 0)
        {
            pthread_exit(&threads[i]);
        }
    }
}
