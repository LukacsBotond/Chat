#include "../common/packages.h"
#include "../common/comon.h"
#include "decode.h"

using namespace std;

void decodeRevc(vector<char> buf)
{
    int kezd=0;
    switch (buf.at(0))
    {
    case 1://all-parancs erkezett
        kezd = 5;
        break;
    case 5:
        kezd = 1;
        break;

    default:
        break;
    }
     string uzenet(buf.begin()+kezd,buf.end());
    cout<<"SERVER>:"<<uzenet<<endl;
}