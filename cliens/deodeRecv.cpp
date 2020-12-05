#include "../common/packages.h"
#include "../common/comon.h"
#include "decode.h"

using namespace std;

void decodeRevc(vector<char> buf)
{

    switch (buf.at(0))
    {
    case 10: //visszaigazolo uzenet
        if (buf.at(1) == 'O' && buf.at(2) == 'K')
        {
            if (semop(semid, &up, 1) < 0)
            {
                cout << "Sem up error" << endl;
                return;
            }
        }
        else
        {
            unsigned int sorszam = getSorszam(buf);
            string pack;
        }
        break;

    default:
        break;
    }
}