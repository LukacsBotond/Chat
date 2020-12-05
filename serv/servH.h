#ifndef servH_H
#define servH_H

#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <algorithm>
#include <vector>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

struct thread_data
{
    int clientSocket;
    int id;
};

inline int semid;
inline struct sembuf down = {0, -1, 0};
inline struct sembuf up= {0, +1, 0};

#define NUM_THREADS 50
inline pthread_t threads[NUM_THREADS];
inline bool szal[NUM_THREADS];
inline struct thread_data td[NUM_THREADS];
inline std::string nevek[NUM_THREADS] = {""};

inline sockaddr_in client;
inline socklen_t clientSize = sizeof(client);
inline int listening;

bool nameSet(int clientSocket, int id);

void *Communicate(void *threadarg);

void SendNames(int sock);

void sendAll(char* data);

int elsoUresSzal();

void semStart();
void startConn();

void printNevek();

void threadExit(int id, int sock);
void kilep(void);

#endif