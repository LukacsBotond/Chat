#include "servH.h"
#include "../common/packages.h"
#include "../common/comon.h"

using namespace std;

void sendAll(string csomag)
{
    semdown();
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
    semup();
}

void SendNames(int sock)
{
    semdown();
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
    semup();
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

    string RetPack;
    semdown();
    auto it = find(begin(nevek), end(nevek), cimzett);
    int x = it - begin(nevek);
    if (x >= NUM_THREADS)
    { //nincs benne a nev
        cout << "Nincs ilyen felhasznalo" << endl;
        RetPack = RetRegPackageGEN('9' + 1, "Error, nincs ilyen aktiv felhasznalo");
        if (!sendPack(sock, RetPack))
        {
            cout << "hiba a kuldes soran\n";
        }
        semup();
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
    semup();
}

void SendFile(vector<char> buf, int sock)
{
    unsigned int sorszam = getSorszam(buf);
    unsigned int utCsomag = 0;
    string cimzettbad = "";
    string cimzett = "";
    string parancs = "";
    string fajlnev = "";
    list<vector<char>> csomagvar;
    //elso csomagba nem erkezik a fajlbol
    if (sorszam == 0)
    {
        //tipus, sorszam kihagyasa
        string adat(buf.begin() + 15, buf.end());
        cout << "adat: " << adat << endl;
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
            cimzettbad = tmp;
            //nev vegerol a 0-k levevese
            for (int i = 0; i < 11; ++i)
            {
                if (cimzettbad[i] == '\0')
                    break;
                cimzett += cimzettbad[i];
            }
        }
        string csomag(buf.begin(), buf.end());
        semdown();
        vector<int> kliensek = FindClinets(parancs, cimzett);
        if (kliensek.size() == 0)
        {
            csomag = RetRegPackageGEN('9' + 2, "ERROR: a cimzett/ek nem elerhatoek");
            if (!sendPack(sock, csomag))
            {
                cout << "Sikertelen kuldes" << endl;
            }
        }
        else
        {
            sendVector(kliensek, csomag, utCsomag);
        }

        semup();
        cout << "fajlnev: " << fajlnev << " parancs " << parancs << " cimzett: " << cimzett << endl;
        utCsomag = 1;
    }

    while (true)
    {
        vector<char> buf(514);
        int res = recv(sock, buf.data(), 514, 0);
        string csomag(buf.begin(), buf.end());
        //cout << "res: " << res << endl;
        while (res < 514)
        {
            vector<char> bufextra(514);
            int extra = recv(sock, bufextra.data(), 514 - res, 0);
            cout << "extra res: " << extra << endl;
            for (int i = 0; i < extra; i++)
            {
                buf.at(i+res) = bufextra.at(i);
            }
            res += extra;
        }
        if (!resCheck(res))
        {
            cout << "EXIT RES\n";
            return;
        }
        sorszam = getSorszam(buf);
        semdown();
        vector<int> kliensek = FindClinets(parancs, cimzett);
        if (sorszam == UINT32_MAX)
        { //utolso csomag
            csomag = FilePackGen('3', "END", UINT32_MAX);
            utCsomag = 0;
            sendVector(kliensek, csomag, utCsomag);
            semup();
            return;
        }
        if (kliensek.size() == 0)
        {
            csomag = RetRegPackageGEN('9' + 2, "ERROR: a cimzett/ek nem elerhatoek");
            if (!sendPack(sock, csomag))
            {
                cout << "Sikertelen kuldes" << endl;
            }
            semup();
            return;
        }

        //egy nagyobb csomag erkezett, eltaroljuk, majd kesobb
        //elkuldjuk
        if (!correctPack(sorszam, utCsomag))
        {
            cout << "Skipp\n";
            csomagvar.push_back(buf);
        }
        else
        { //jo csomag, amit el kell kuldeni egy kliensnek
            sendVector(kliensek, csomag, utCsomag);
        }
        KovCsomag(csomagvar, kliensek, utCsomag);
        semup();
    }
}

void sendVector(vector<int> kliensek, string csomag, unsigned int &utCsomag)
{
    for (unsigned int i = 0; i < kliensek.size(); i++)
    {
        if (!sendPack(kliensek.at(i), csomag))
            cout << "Sikertelen kuldes" << endl;
    }
    utCsomag++;
}

void SendGroup(vector<char> buf, int sock)
{
    string csomag(buf.begin(), buf.end());
    string cimzett = "";
    for (int i = 5; i < 16; ++i)
    {
        if (buf.at(i) == '\0')
            break;
        cimzett += buf[i];
    }
    cout<<"csoport: "<<cimzett<<endl;
    string RetPack;
    semdown();
    auto it = csoportMap.find(cimzett);
    if (it == csoportMap.end())
    {
        cout << "Nincs ilyen CsoportNev\n";
        RetPack = RetRegPackageGEN('9' + 1, "Error, nincs ilyen Csoport");
        if (!sendPack(sock, RetPack))
        {
            cout << "hiba a kuldes soran\n";
        }
        semup();
        return;
    }
    vector<int> kliensek;
    for (auto const i : it->second)
    {
        kliensek.push_back(i);
    }
    unsigned int csomagSzam = 0;
    sendVector(kliensek, csomag, csomagSzam);

    semup();
}
