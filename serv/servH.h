#ifndef servH_H
#define servH_H

#include "../common/comon.h"
#include "../common/packages.h"

#include <algorithm>
#include <cstring>
#include <map>
#include <list>

#include <pthread.h>
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

inline std::map<std::string,std::list<int>> csoportMap;

inline sockaddr_in client;
inline socklen_t clientSize = sizeof(client);
inline int listening;

void *Communicate(void *threadarg);

//all tipusu uzenetkent kulve vissza, de csak annak aki kerdezte
void SendNames(int sock);

void sendAll(std::string csomag);

void SendPriv(std::vector<char> buf,int sock,int id);

//mivel a szam egy klienssel foglalokozik csak
//es a kliens nem tud kuldeni ameddig az elozo kuldes be nem
//fejezodik, igy ha fajl csomag erkezik
//akkor addig erkezik a fajl mig utolso csomag jelzest nem kap
void SendFile(std::vector<char> buf,int sock);

void sendVector(std::vector<int> kliensek,std::string csomag,unsigned int &utCsomag);

//////////////////////////init.cpp
bool nameSet(int clientSocket, int id);
void CreateGroup(std::vector<char> buf,int sock);
void JoinGroup(std::vector<char> buf,int sock);
void SendGroup(std::vector<char> buf, int sock);

int elsoUresSzal();

void semStart();
void startConn();

void printNevek();

void threadExit(int id, int sock);
void kilep(void);



///////////////////////////// SUpport functions

unsigned int getSorszam(std::vector<char> buf);

bool decodeRECV(std::vector<char> buf,int sock,int id);

void KovCsomag(std::list<std::vector<char>> &csomagok,std::vector<int> kliensek,unsigned int &utCsomag);

std::vector<int> FindClinets(std::string parancs, std::string cimzett);

std::string getNev(std::string namebad);

void semup();
void semdown();
#endif