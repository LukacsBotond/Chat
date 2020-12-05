#include "../common/packages.h"
#include "decode.h"
#include <sstream>

using namespace std;

//a felhasznalo altal megadott parancsot ertelmezi
bool decodeCommand(string userInput)
{
    int kezdo = 0;
    cout << "DECODE\n";
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
    return false;
}

bool AllSend(string uzenet)
{
    int kezdo = 0;
    int sorszam = 0;
    int osszcsomag = (uzenet.length() / 507) + 1;

    int res;
    for (int i = 0; i < osszcsomag; i++)
    {
        string pack;
        //send
        try
        {   //utolso csomag
            if (i == osszcsomag - 1)
            {
                pack = AllPackageGEN(uzenet.substr(kezdo, 507), sorszam);
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
        //nincs mit kuldeni
        if (pack.length() == 0)
        {
            cout << "nincs mit kuldeni";
            continue;
        }

        res = send(sock, pack.c_str(), 512, 0);
        cout << "res: " << res << endl;
        if (res == -1)
        {
            cout << "Nem sikerult a szerverhez elkuldeni \n";
            continue;
        }

        //wait for the server to respond
        //update after it was succesfully recived
        kezdo += 507;
        sorszam++;
    }

    return true;
}
