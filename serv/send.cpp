#include "servH.h"
#include "../common/packages.h"
#include "../common/comon.h"

using namespace std;

void sendAll(string csomag)
{
    if (semop(semid, &down, 1) < 0)
    {
        cout << "Sem down error" << endl;
        exit(-1);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (!szal[i])
        {
            continue;
        }
        if (!sendPack(td[i].clientSocket, csomag))
        {
            cout << "hiba a kuldes soran\n";
        }
    }
    if (semop(semid, &up, 1) < 0)
    {
        cout << "Sem up error" << endl;
        exit(-1);
    }
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
    int kezdo = 0;
    string returnPack;
    unsigned int osszcsomag = (RetNev.length() / 511) + 1;
    for (unsigned int i = 0; i < osszcsomag; i++)
    {
        returnPack = RetRegPackageGEN('5', RetNev.substr(kezdo, 511));
        if (!sendPack(sock, returnPack))
        {
            cout << "HIBA NEV kuldeskor\n";
            return;
        }
    }
    if (semop(semid, &up, 1) < 0)
    {
        cout << "Sem up error" << endl;
        return;
    }
}

void SendPriv(vector<char> buf, int sock, int id)
{
    string csomag(buf.begin(), buf.end());
    string cimzett = "";
    for (int i = 5; i < 16; ++i)
    {
        if (buf.at(i) == '\0')
            break;
        cimzett += buf[i];
    }
    cout << "Cimzett: " << cimzett << endl;
    if (semop(semid, &down, 1) < 0)
    {
        cout << "Sem down error" << endl;
        return;
    }
    string RetPack;

    auto it = find(begin(nevek), end(nevek), cimzett);
    int x = it - begin(nevek);
    if (x < NUM_THREADS)
        cout << "Van ilyen felhasznalo" << endl;
    else
    { //nincs benne a nev
        cout << "Nincs ilyen felhasznalo" << endl;
        RetPack = RetRegPackageGEN('9' + 1, "Error, nincs ilyen aktiv felhasznalo");
        if (!sendPack(sock, RetPack))
        {
            cout << "hiba a kuldes soran\n";
        }
        if (semop(semid, &up, 1) < 0)
        {
            cout << "Sem up error" << endl;
            return;
        }
        return;
    }
    //nevev kicserelese a cimzettrol a feladora
    for (int i = 5; i < 15; i++)
    {
        csomag[i] = nevek[id][i - 5];
    }
    csomag[15] = ':';
    if (!sendPack(td[x].clientSocket, csomag))
    {
        cout << "hiba a kuldes soran\n";
    }
    if (semop(semid, &up, 1) < 0)
    {
        cout << "Sem up error" << endl;
        return;
    }
}

void SendFile(vector<char> buf, int sock)
{
    unsigned int sorszam = getSorszam(buf);
    unsigned int utCsomag = 0;
    string cimzett="";
    string parancs="";
    string fajlnev="";
    list<vector<char>> csomagvar;
    //elso csomagba nem erkezik a fajlbol
    if (sorszam == 0)
    {
        //tipus, sorszam kihagyasa
        string adat(buf.begin() + 15, buf.end());
        cout<<"adat: "<<adat<<endl;
        string tmp;
        stringstream ss(adat);
        ss >> tmp; //-file skip

        ss >> tmp; //fajlnev
        fajlnev = tmp;

        ss >> tmp; //parancs
        parancs = tmp;

        if (parancs != "-a")
        {
            ss >> tmp;
            cimzett = tmp;
        }
        cout << "fajlnev: " << fajlnev << " parancs " << parancs << " cimzett: " << cimzett << endl;
    }
    /*
    bool vege = false;
    while (vege)
    {
        vector<char> buf(550);
        int res = recv(sock, buf.data(), 550, 0);
        string csomag(buf.begin(), buf.end());
        cout << "res: " << res << endl;
        if (!resCheck(res))
        {
            cout << "EXIT RES\n";
            return;
        }
        sorszam = getSorszam(buf);
        if (sorszam == UINT32_MAX)
        { //utolso csomag
            break;
        }
        //egy nagyobb csomag erkezett, eltaroljuk, majd kesobb
        //elkuldjuk
        if (!correctPack(sorszam, utCsomag))
        {
            csomagvar.push_back(buf);
            continue;
        }
        else
        { //jo csomag, amit el kell kuldeni egy kliensnek
            if (semop(semid, &down, 1) < 0)
            {
                cout << "Sem down error" << endl;
                throw out_of_range("");
            }
            vector<int> kliensek = FindClinets(parancs, cimzett);
            for (unsigned int i = 0; i < kliensek.size(); i++)
            {
                if (!sendPack(kliensek.at(i), csomag))
                {
                    cout << "Sikertelen kuldes" << endl;
                }
            }
            if (semop(semid, &up, 1) < 0)
            {
                cout << "Sem up error" << endl;
                throw out_of_range("");
            }
        }
    }
    */
}
