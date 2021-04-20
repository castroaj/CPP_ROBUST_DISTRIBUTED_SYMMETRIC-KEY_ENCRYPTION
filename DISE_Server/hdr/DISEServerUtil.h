#ifndef DISE_SERVER_UTIL_H
#define DISE_SERVER_UTIL_H
    #include "DISEServerDriver.h"
    #include "Environment.h"

    int encrypt(unsigned char* message, int msgLen, unsigned char* key, unsigned char* encryptedMessage);
    int decrypt(unsigned char* message, int msgLen, unsigned char* key, unsigned char* decryptedMessage);
    unsigned int cryptoHash(unsigned char* data, int dataLen, unsigned char* result);
    void printServerKeysToUse(QMap<int, QList<int>*>* serverKeysToUse);
    QMap<int, unsigned char*>* encryptWithKeyList(QList<int>* keyList, unsigned char* message, int msgSize, QMap<int, unsigned char*>* completeKeyList);
    void randomNumberWithSeed(unsigned char* seed, int seedLen, unsigned char* result, int resultSize);
    QList<int>* getParticipantServerList(int n, int t, int machineNum);
    QMap<int, QList<int>*>* getParticipantServerKeyMap(QList<int>* partipantServers, Environment* environment);

#endif