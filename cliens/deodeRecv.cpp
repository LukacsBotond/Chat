#include "../common/packages.h"
#include "../common/comon.h"
#include "decode.h"

using namespace std;

void decodeRevc(vector<char> buf)
{
    int kezd = 0;
    stop = false;
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
    case 2:
    {
        cout << "Csoporttol erkezik az uzenet: \n";
        string csoport(buf.begin() + 5, buf.begin() + 15);
        cout << csoport << ": ";
        kezd = 15;
        break;
    }
    case 3:
        cout << "Fajl erkezik" << endl;
        reciveFile(buf);
        return;
        break;

    case 5:
        cout << "Lista: ";
        kezd = 1;
        break;

    case 9:
        cout << "szerver visszateritett: ";
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
    cout << uzenet << endl;
}

void reciveFile(vector<char> bufbe)
{
    unsigned int sorszam = getSorszam(bufbe);
    unsigned int utCsomag = 0;
    string parancs = "";
    string fajlnev = "";
    list<vector<char>> csomagvar;
    if (sorszam == 0)
    {
        string cimzettbad = "";
        //tipus, sorszam kihagyasa
        string adat(bufbe.begin() + 15, bufbe.begin() + 512);
        cout << "adat: " << adat << endl;
        string tmp;
        stringstream ss(adat);
        ss >> tmp; //-file skip
        ss >> tmp; //fajlnev
        fajlnev = tmp;
        utCsomag = 1;
    }
    ofstream file(fajlnev, std::ifstream::binary);
    if (file.fail())
    {
        cout << "Fajl iras hiba" << endl;
    }
    vector<char> buff(514);
    while (true)
    {
        int res = recv(sock, buff.data(), 514, 0);
        //cout<<"res:"<<res<<endl;
        while (res < 514)
        {
            vector<char> bufextra(514);
            int extra = recv(sock, bufextra.data(), 514 - res, 0);
            if (!resCheck(extra))
            {
                cout << "EXIT RES\n";
                return;
            }
            cout << "extra res:" << extra << endl;
            for (int i = 0; i < extra; i++)
            {
                buff.at(res + i) = bufextra.at(i);
            }
            res += extra;
        }
        std::vector<char> data(buff.begin() + 5, buff.end());
        if (!resCheck(res))
        {
            cout << "EXIT RES\n";
            return;
        }
        sorszam = getSorszam(buff);
        if (sorszam == UINT32_MAX)
        { //utolso csomag
            cout << "VEGE a fajl kuldesnek\n";
            break;
        }
        //egy nagyobb csomag erkezett, eltaroljuk, majd kesobb
        //elkuldjuk
        if (!correctPack(sorszam, utCsomag))
        {
            cout << "SKIPP " << sorszam << " " << utCsomag << endl;
            csomagvar.push_back(buff);
        }
        else
        { //jo csomag, ki kell irni
            file.write(data.data(), data.size());
            utCsomag++;
        }
        kovCsomagKliens(csomagvar, file, utCsomag);
    }

    file.close();
}

void kovCsomagKliens(list<vector<char>> &csomagvar, ofstream &file, unsigned int &utCsomag)
{
    //cout << "keres" << endl;
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
                cout << "talal" << endl;
                cin.ignore();
                talalt = true;
                file.write(it.data(), it.size());
                csomagvar.remove(it);
                utCsomag++;
            }
        }
    }
}
