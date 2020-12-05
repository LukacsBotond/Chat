#ifndef DECODE_H
#define DECODE_H

#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <vector>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

inline int sock;
inline unsigned int sorszam = 0;

inline int semid;
inline struct sembuf down = {0, -1, 0};
inline struct sembuf up= {0, +1, 0};

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

bool sendPack(std::string pack);


void decodeRevc(std::vector<char> buf);

#endif