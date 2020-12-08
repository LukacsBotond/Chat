#include "../common/packages.h"
#include "../common/comon.h"
#include "decode.h"
#include <sstream>

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
                pack = AllPackageGEN(uzenet.substr(kezdo), UINT32_MAX);
            }
            else
            {
                pack = AllPackageGEN(uzenet.substr(kezdo, 507), sorszam);
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
    kezdo+=buf.length();
    ss >> buf; //a nev
    kezdo+=buf.length()+1;
    fogado = buf;
    int sorszam = 0;
    int osszcsomag = (userInput.length() / 497) + 1;
    string adat = userInput.substr(kezdo);
    for (int i = 0; i < osszcsomag; i++)
    {
        string pack;
        //send
        try
        { //utolso csomag
            if (i == osszcsomag - 1)
            {
                pack = PrivPackageGEN('0',UINT32_MAX,fogado,adat);
            }
            else
            {
                pack = PrivPackageGEN('0',sorszam,fogado,adat);
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
