#include "../common/packages.h"
#include "../common/comon.h"
#include "decode.h"

using namespace std;

void decodeRevc(vector<char> buf)
{
    int kezd = 0;
    switch (buf.at(0))
    {
    case 0:
        kezd = 5;
        cout << "Privat uzenet: ";
        break;
    case 1: //all-parancs erkezett
        cout << "Globalis uzenet: ";
        kezd = 5;
        break;
    case 3:
        cout << "Fajl erkezik" << endl;
        reciveFile(buf);
        return;
        break;

    case 5:
        cout << "Lista: ";
        kezd = 1;
        break;

    case 10:
        cout << "ERROR:";
        stop = true;
        kezd = 1;
        break;

    default:
        cout << "ismeretlen parancs" << endl;
        break;
    }
    string uzenet(buf.begin() + kezd, buf.end());
    cout << "SERVER>:" << uzenet << endl;
}

void reciveFile(vector<char> buf)
{
    unsigned int sorszam = getSorszam(buf);
    unsigned int utCsomag = 0;
    string cimzettbad = "";
    string cimzett = "";
    string parancs = "";
    string fajlnev = "";
    list<vector<char>> csomagvar;
    if (sorszam == 0)
    {
        //tipus, sorszam kihagyasa
        string adat(buf.begin() + 15, buf.begin() + 512);
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
        cout << "fajlnev: " << fajlnev << " parancs " << parancs << " cimzett: " << cimzett << endl;
        utCsomag = 1;
    }

    ofstream file(fajlnev, std::ifstream::binary);
    if (file.fail())
    {
        cout << "Fajl iras hiba" << endl;
    }

    while (true)
    {
        //cout << "FILE RECV:" << endl;
        vector<char> buf(514);
        int res = recv(sock, buf.data(), 514, 0);
        std::vector<char> data(buf.begin() + 5, buf.end());
        if (!resCheck(res))
        {
            cout << "EXIT RES\n";
            return;
        }
        sorszam = getSorszam(buf);
        cout << "Sorszam " << sorszam << endl;
        if (sorszam == UINT32_MAX)
        { //utolso csomag
            cout << "VEGE a fajl kuldesnek\n";
            return;
        }

        //egy nagyobb csomag erkezett, eltaroljuk, majd kesobb
        //elkuldjuk
        if (!correctPack(sorszam, utCsomag))
        {
            cout << "SKIPP" << endl;
            csomagvar.push_back(buf);
            continue;
        }
        else
        { //jo csomag, ki kell irni
            file.write(data.data(), data.size());
            utCsomag++;
        }
        kovCsomagKliens(csomagvar,file,utCsomag);
    }


    file.close();
}

void kovCsomagKliens(list<vector<char>> &csomagvar, ofstream &file,unsigned int &utCsomag)
{
    cout<<"keres"<<endl;
    unsigned int sorszam;
    bool talalt = true;
    while (talalt)
    {
        talalt = false;
        for (auto it : csomagvar)
        {
            sorszam = getSorszam(it);
            if (sorszam == utCsomag)
            {
                cout<<"talal"<<endl;
                talalt = true;
                file.write(it.data(),it.size());
                csomagvar.remove(it);
                utCsomag++;
                break;
            }
        }
    }
}