#include "../common/packages.h"
#include "../common/comon.h"
#include "decode.h"
#include <sstream>
#include <fstream>

using namespace std;

//a felhasznalo altal megadott parancsot ertelmezi
bool decodeCommand()
{
    int kezdo = 0;
    string command;
    string param1, param2;
    string adat;

    string buf;
    stringstream ss(userInput);
    //elso szo kivevese
    ss >> buf;
    if (buf == "-all")
    {
        kezdo = 5;
        return AllSend(userInput.substr(kezdo));
    }
    if (buf == "-a")
    {
        kezdo = 3;
        return AllSend(userInput.substr(kezdo));
    }
    if (buf == "-l" || buf == "-list")
    {
        return ListSend();
    }
    if (buf == "-priv" || buf == "-p")
    {
        return SendPriv();
    }
    if (buf == "-file" || buf == "-f")
    {
        return SendFile();
    }

    return false;
}

bool ListSend()
{
    string retPack = RetRegPackageGEN('5', "list");
    return sendPack(sock, retPack);
}

bool AllSend(string uzenet)
{
    int kezdo = 0;
    int sorszam = 0;
    int osszcsomag = (uzenet.length() / 507) + 1;
    for (int i = 0; i < osszcsomag; i++)
    {
        string pack;
        //send
        try
        { //utolso csomag
            if (i == osszcsomag - 1)
            {
                pack = AllPackageGEN('1', uzenet.substr(kezdo), UINT32_MAX);
            }
            else
            {
                pack = AllPackageGEN('1', uzenet.substr(kezdo, 507), sorszam);
            }
        }
        catch (out_of_range &e)
        {
            std::cout << e.what() << '\n';
            return false;
        }
        catch (length_error &e)
        {
            std::cout << e.what() << '\n';
            return false;
        }

        if (!sendPack(sock, pack))
        {
            return false;
        }

        kezdo += 507;
        sorszam++;
    }
    return true;
}

bool SendPriv()
{
    string buf;
    string fogado;
    stringstream ss(userInput);
    int kezdo = 0;
    ss >> buf; //ez a -priv lesz
    kezdo += buf.length();
    ss >> buf; //a nev
    kezdo += buf.length() + 1;
    fogado = buf;
    int sorszam = 0;
    int osszcsomag = (userInput.length() / 497) + 1;
    string adat = userInput.substr(kezdo);
    for (int i = 0; i < osszcsomag; i++)
    {
        if (stop)
        {
            break;
        }
        string pack;
        //send
        try
        { //utolso csomag
            if (i == osszcsomag - 1)
            {
                pack = PrivPackageGEN('0', UINT32_MAX, fogado, adat);
            }
            else
            {
                pack = PrivPackageGEN('0', sorszam, fogado, adat);
            }
        }
        catch (out_of_range &e)
        {
            std::cout << e.what() << '\n';
            return false;
        }
        catch (length_error &e)
        {
            std::cout << e.what() << '\n';
            return false;
        }

        if (!sendPack(sock, pack))
        {
            return false;
        }

        kezdo += 497;
        sorszam++;
    }
    return true;

    return true;
}

bool SendFile()
{
    string buf;
    string fajlnev;
    string csomag = "";
    unsigned int sorszam = 1;
    //elso csomag
    string tmp;
    stringstream ss(userInput);
    ss >> tmp; //-file skip

    ss >> tmp; //fajlnev
    fajlnev = tmp;
    cout << fajlnev << endl;
    ifstream sendFile(fajlnev, std::ifstream::binary);
    if (sendFile.fail())
    {
        cout << "nincs ilyen fajl\n";
        return false;
    }

    csomag = PrivPackageGEN('3', 0, "fill", userInput);
    if (!sendPack(sock, csomag))
    {
        return false;
    }
    vector<char> resz(550);
    while (!sendFile.eof())
    {
        if (stop)
        {
            break;
        }
        cout << "sending...." << sorszam << endl;
        sendFile.read(resz.data(), 507);
        string reszCs(resz.begin(), resz.end());
        //cout<<reszCs<<endl;
        csomag = FilePackGen('3', reszCs, sorszam);
        if (!sendPack(sock, csomag))
        {
            cout << "valami nem jo" << endl;
            return false;
        }
        sorszam++;
    }
    //vege a fajlnak
    cout << "Elkuldve\n";
    csomag = PrivPackageGEN('3', UINT32_MAX, "fill", "fill");
    if (!sendPack(sock, csomag))
    {
        cout << "valami nem jo" << endl;
        return false;
    }
    sendFile.close();

    return true;
}
