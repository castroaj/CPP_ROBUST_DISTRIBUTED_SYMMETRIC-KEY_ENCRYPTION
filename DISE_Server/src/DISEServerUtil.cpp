#include "../hdr/DISEServerUtil.h"

int encrypt(unsigned char* message, int msgLen, unsigned char* key, unsigned char* encryptedMessage) 
{
    int ivLen = 12;    
    unsigned char iv[ivLen];
    memset(iv, 1, ivLen);
    int encLen = 0;
    int finalLen = 0;

    /* Create and initialise the context */
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    // init encryption operation
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, iv);

    // update the encrypted message
    EVP_EncryptUpdate(ctx, encryptedMessage, &encLen, message, msgLen);

    // finilize the encryption
    EVP_EncryptFinal_ex(ctx, encryptedMessage + encLen, &finalLen);

    encLen += finalLen;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return encLen;
}

int decrypt(unsigned char* message, int msgLen, unsigned char* key, unsigned char* decryptedMessage) 
{
    // TODO FIX THIS AND SEND AN IV WITH KEY IN DEALER STORE IN ENV :'(
    int ivLen = 12;   
    unsigned char iv[ivLen];
    memset(iv, 1, ivLen);

    int decLen = 0;
    int finalLen = 0;

    // Create Context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    // initialize decription operation
    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, iv );

    // update the decrypted message
    EVP_DecryptUpdate(ctx, decryptedMessage, &decLen, message, msgLen);

    // finilize the enc
    EVP_DecryptFinal_ex(ctx, decryptedMessage + decLen, &finalLen);

    decLen += finalLen;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return decLen;
}

unsigned int cryptoHash(unsigned char* data, int dataLen, unsigned char* result) 
{
    unsigned int resultLen;

    // EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_MD_CTX *mdctx = EVP_MD_CTX_create();

    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);

    EVP_DigestUpdate(mdctx, data, dataLen);

    // result = (unsigned char *)OPENSSL_malloc(EVP_MD_size( EVP_sha256()));

    EVP_DigestFinal_ex(mdctx, result, &resultLen);

    EVP_MD_CTX_free(mdctx);
    
    return resultLen;
}

void randomNumberWithSeed(unsigned char* seed, int seedLen, unsigned char* result, int resultSize) 
{
    // RAND_seed(seed, seedLen);
    // RAND_bytes(result, resultSize);
    for (int i = 0; i < resultSize; i++) 
    {
        result[i] = seed[i % seedLen] * 2;
    }

    // Poco::HMACEngine<Poco::Crypto::SHA256> hmac = new HMACEngine(seed, seedLen);
    // Poco::HMACEngine<SHA256Engine> hmac{secretKey};
    // hmac.update(string);

    // hmac.updateImpl(result, resultSize);

    // mbedtls_hmac_drbg_context *ctx;
    // mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

    // mbedtls_hmac_drbg_init(  ctx );

	// mbedtls_hmac_drbg_init(ctx);
    // mbedtls_hmac_drbg_seed_buf(ctx, mbedtls_md_info_from_type(md_type), seed, seedLen);
}


QMap<int, unsigned char*>* encryptWithKeyList(QList<int>* keyList, unsigned char* message, int msgSize, QMap<int, unsigned char*>* completeKeyList)
{
    QMap<int, unsigned char*>* partialResults = new QMap<int, unsigned char*>();

    // for each key in the keys to use list
    #pragma omp parallel for default(none) shared(completeKeyList, keyList, message, msgSize, partialResults) 
    for (int i = 0; i < keyList->size(); i++)
    {
        unsigned char* key = completeKeyList->value(keyList->at(i));
        // where the encrypted or decrypted msg will be saved
        unsigned char* resultMessage = (unsigned char *)malloc(MAX_CIPHER_LEN);

        encrypt(message, msgSize, key, resultMessage);
        
        partialResults->insert(keyList->at(i), resultMessage);
    }

    return partialResults;
}

QList<int>* getParticipantServerList(int n, int t, int machineNum)
{
    // Random combination of t other participant servers
    QList<int>* partipantServerNumbers = new QList<int>();
    // Populate list of server options
    QList<int>* serverOptions = new QList<int>();
    for (int i = 0; i < n; i++)
    {
        // contains every server excluding the honest initiator
        if (i != machineNum)
        {
            serverOptions->append(i);
        }
    }

    // select participants randomly
    srand(time(NULL));
    for (int i = 0; i < t; i++)
    {
        int partipantIndex = rand() % serverOptions->size();
        partipantServerNumbers->append(serverOptions->at(partipantIndex));
        serverOptions->removeAt(partipantIndex);
    }

    // Free memory
    delete serverOptions;

    return partipantServerNumbers;
}

void printServerKeysToUse(QMap<int, QList<int>*>* serverKeysToUse)
{
    // iterate through n machines held keys
    for (QMap<int, QList<int>*>::iterator serverIter = serverKeysToUse->begin(); serverIter != serverKeysToUse->end(); ++serverIter)
    {
        std::cout << "\t" << serverIter.key() << ": ";

        // print key values in omega row
        QList<int>* keysToUse = serverIter.value();

        for (int i = 0; i < keysToUse->size(); i++)
            std::cout << keysToUse->at(i) << " ";
        
        std::cout << "\n";
    }
}


QMap<int, QList<int>*>* getParticipantServerKeyMap(QList<int>* participantServers, Environment* environment)
{

    QMap<int, QList<int>*>* serverKeysToUse = new QMap<int, QList<int>*>();
    QMap<int, int>* numberOfKeysAssigned = new QMap<int, int>();
    
    // Zero out the num keys assigned
    for (int i = 0; i < participantServers->size(); i++)
    {
        numberOfKeysAssigned->insert(participantServers->at(i), 0);
        serverKeysToUse->insert(participantServers->at(i), new QList<int>());
    }

    int honestInit = environment->get_machine_num();
    serverKeysToUse->insert(honestInit, new QList<int>());

    // Go through each key to decide what machines will use what key
    for (int keyId = 0; keyId < environment->get_total_key_num(); keyId++)
    {
        // if the honest doesn't own this key assign it to two servers
        if (!environment->server_owns_key(honestInit, keyId))
        {
            // Assign the key to the two servers with the least keys that hold the key
            int firstLeastKeysAssigned = INT_MIN;
            int firstLeastKeysAssignedId = INT_MIN;
            int secondLeastKeysAssigned = INT_MIN;
            int secondLeastKeysAssignedId = INT_MIN;
            // for each partipating server
            for (int i = 0; i < participantServers->size(); i++)
            {
                if (environment->server_owns_key(participantServers->at(i), keyId))
                {
                    int numKeysAssigned = numberOfKeysAssigned->value(participantServers->at(i));
                    if (firstLeastKeysAssigned == INT_MIN) // First least keys unset
                    {
                        firstLeastKeysAssigned = numKeysAssigned;
                        firstLeastKeysAssignedId = participantServers->at(i);
                    }
                    else if (secondLeastKeysAssigned) // Second Least unset
                    {
                        secondLeastKeysAssigned = numKeysAssigned;
                        secondLeastKeysAssignedId = participantServers->at(i);
                    }
                    else
                    {
                        if (numKeysAssigned > secondLeastKeysAssigned && numKeysAssigned > firstLeastKeysAssigned)
                        {
                            secondLeastKeysAssigned = firstLeastKeysAssigned;
                            secondLeastKeysAssignedId = firstLeastKeysAssignedId;
                            firstLeastKeysAssigned = numKeysAssigned;
                            firstLeastKeysAssignedId = participantServers->at(i);
                        }
                        else if (numKeysAssigned > secondLeastKeysAssigned)
                        {
                            secondLeastKeysAssigned = numKeysAssigned;
                            secondLeastKeysAssignedId = participantServers->at(i);
                        }
                    }
                }
            }

            // update the num keys assigned
            int newAmountOfKeysHeld = numberOfKeysAssigned->value(firstLeastKeysAssignedId);
            numberOfKeysAssigned->insert(firstLeastKeysAssignedId, newAmountOfKeysHeld++);
            newAmountOfKeysHeld = numberOfKeysAssigned->value(secondLeastKeysAssignedId);
            numberOfKeysAssigned->insert(secondLeastKeysAssignedId, newAmountOfKeysHeld++);

            // Assign keys
            // std::cout << "(" << firstLeastKeysAssignedId << " " << secondLeastKeysAssignedId << " " << keyId << ") " << std::flush;
            QList<int>* curKeysFirst = serverKeysToUse->value(firstLeastKeysAssignedId);
            QList<int>* curKeysSecond = serverKeysToUse->value(secondLeastKeysAssignedId);
            curKeysFirst->append(keyId);
            curKeysSecond->append(keyId);
        }
        else // owned by the honest init
        {
            QList<int>* curKeys = serverKeysToUse->value(honestInit);
            curKeys->append(keyId);
        }
    }

    // Free memory
    delete numberOfKeysAssigned;

    return serverKeysToUse;
}
