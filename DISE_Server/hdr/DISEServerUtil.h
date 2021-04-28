/*************************************************************
 * Header file detailing the Dise Server Utily Functions
 * 
 * @file DISEServerUtil.h
 * @author Alexander Castro, Garrett Christian,
 *         Rachel Litscher , Steve Chang
 * 
 *************************************************************/ 
#ifndef DISE_SERVER_UTIL_H
#define DISE_SERVER_UTIL_H
    #include "DISEServerDriver.h"
    #include "Environment.h"

    /******************************************************************
     * OPENSSL wrapper function that that takes a message buffer and 
     * encrypts it with the given key and stores it in the encryptedMessage
     * buffer. This uses AES/256/GCM.
     * 
     * @param message is the plaintext to be encrypted
     * @param msgLen is the length of the plaintext
     * @param key is the AES key for the encryption
     * @param encryptedMessage is the resulting cipertext
     * @return the length of the encrypted message
     *****************************************************************/
    int encrypt(unsigned char* message, int msgLen, unsigned char* key, unsigned char* encryptedMessage);

    /******************************************************************
     * OPENSSL wrapper function that that takes a message buffer and 
     * decrypts it with the given key and stores it in the decryptedMessage
     * buffer. This uses AES/256/GCM.
     * 
     * @param message is the ciphertText to be decrypted
     * @param msgLen is the length of the cipherText
     * @param key is the AES key for the decryption
     * @param decryptedMessage is the resulting plaintext
     * @return the length of the decrypted message
     *****************************************************************/
    int decrypt(unsigned char* message, int msgLen, unsigned char* key, unsigned char* decryptedMessage);

    /******************************************************************
     * OPENSSL wrapper function that takes the given data buffer and 
     * hashes it using the sha256 algorithm and stores it in the result
     * buffer.
     * 
     * @param data is the input data to be hashed
     * @param dataLen is the length of the input data
     * @param result is the buffer to store the hash
     * @return the length of the resulting hash (256)
     *****************************************************************/
    unsigned int cryptoHash(unsigned char* data, int dataLen, unsigned char* result);

    /******************************************************************
     * Helper Function to print out the serverKeysToUse Data Structure.
     * 
     * @param serverKeysToUse is the structure to print out
     *****************************************************************/
    void printServerKeysToUse(QMap<int, QList<int>*>* serverKeysToUse);

    /******************************************************************
     * Encrypt the given message with the given keylist and create the 
     * partial results map.
     * 
     * @param keyList is the keyList of IDs that will be used for encrpytion
     * @param messgae is the message that will be encrypted
     * @param msgSize is the size of the message
     * @param completeKeyList is the list of actual keys that will be indexed into
     * @return a map containing the partial results
     *****************************************************************/
    QMap<int, unsigned char*>* encryptWithKeyList(QList<int>* keyList, unsigned char* message, int msgSize, QMap<int, unsigned char*>* completeKeyList);

    /******************************************************************
     * Helper function that generates a random number given a seed.
     * Was not successfully implemented due to versioning issues.
     * 
     * @param seed is the seed for the random number generator function
     * @param seedLen is the length of the seed
     * @param result is the buffer to hold the resulting random number
     * @param resultSize is the size of the resulting number
     * 
     *****************************************************************/
    void randomNumberWithSeed(unsigned char* seed, int seedLen, unsigned char* result, int resultSize);

    /******************************************************************
     * Get a list of participating server for a given scheme size and 
     * a given machine number.
     * 
     * @param n is the number of DISE Servers in the scheme
     * @param t is the threshold count for a given scheme
     * @param machineNum is the number of the given machine
     * @return the list of participating servers.
     *****************************************************************/
    QList<int>* getParticipantServerList(int n, int t, int machineNum);

    /******************************************************************
     * Take the participating servers and get the redundent keys that
     * will be assigned to each of those machines. 
     * 
     * @param partipantServers is the participating servers
     * @param environment is the current enviorment of the DISE Servers
     * @return map of server keys to use on each participating server
     *****************************************************************/
    QMap<int, QList<int>*>* getParticipantServerKeyMap(QList<int>* partipantServers, Environment* environment);

#endif