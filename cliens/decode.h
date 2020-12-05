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

inline int sock;

struct thread_data
{
    int sock;
};

inline pthread_t threads[1];
inline struct thread_data td[1];

void sendName();
//megkapja a felhasznalotol a nyers parancsot es
//meghivja a megfelelo send fuggvenyt
bool decodeCommand(std::string userInput);

bool AllSend(std::string uzenet);

#endif