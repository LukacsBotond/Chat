#ifndef DECODE_H
#define DECODE_H

#include "../common/comon.h"
#include "../common/packages.h"
#include <fstream>

inline bool vege = false;
inline bool stop = false;

inline int sock;
inline unsigned int sorszam = 0;

inline std::string userInput;

struct thread_data
{
    int sock;
};

inline pthread_t threads[1];
inline struct thread_data td[1];

void sendName();
//megkapja a felhasznalotol a nyers parancsot es
//meghivja a megfelelo send fuggvenyt
bool decodeCommand();


bool AllSend(std::string uzenet);
bool ListSend();
bool SendPriv();
bool SendFile();

//szervertol kapott adat feldolgozasa
void decodeRevc(std::vector<char> buf);

void reciveFile(std::vector<char> buf);
void kovCsomagKliens(std::list<std::vector<char>> &csomagvar,std::ofstream &file,unsigned int &utCsomag);


void kilep(void);
void MakeSocket();

#endif