#include "../hdr/DISEServer.h"

QT_USE_NAMESPACE

DISEServer::DISEServer(int port, bool debug, Environment* env, QObject* parent) : 
    QObject(parent),
    m_server(new QTcpServer),
    debug(debug),
    port(port),
    environment(env)
{
    mtx = new std::mutex();
    if(m_server->listen(QHostAddress::Any, port))
    {
       connect(m_server, &QTcpServer::newConnection, this, &DISEServer::newConnection);
       if (debug)
           std::cout << "Server Listening on port " << port << std::endl;
    }
    else
    {
        std::cout << "Failed to bind to port " << port << std::endl;
        exit(EXIT_FAILURE);
    }
}

DISEServer::~DISEServer()
{
    foreach (QTcpSocket* socket, connection_set)
    {
        socket->close();
        socket->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();
    delete mtx;
}

void DISEServer::newConnection()
{
    while (m_server->hasPendingConnections())
        appendToSocketList(m_server->nextPendingConnection());
}

void DISEServer::appendToSocketList(QTcpSocket* socket)
{
    connection_set.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &DISEServer::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &DISEServer::discardSocket);
}

void DISEServer::readSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    
    socket->waitForReadyRead(1000);

    QString senderCode = socket->read(1);
    int code = senderCode.toInt();
    
    switch(code)
    {
        case 0: // DEALER MESSAGE
            QTextStream(stdout) << "Dealer Message Recieved" << "\n";
            handleDealer(socket);
            break;
        case 1: // CLIENT MESSAGE
            QTextStream(stdout) << "Client Message Recieved" << "\n";
            handleClient(socket);
            break;
        case 2: // HONEST INITIATOR MESSAGE
            QTextStream(stdout) << "Honest Initiator Message Recieved" << "\n";
            handleHonestInitiator(socket);
            break;
    }
}

void DISEServer::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = connection_set.find(socket);
    if (it != connection_set.end()){
        connection_set.remove(*it);
    }

    std::cout << "Socket Disconnected" << std::endl;
    
    socket->deleteLater();
}

void DISEServer::handleDealer(QTcpSocket* socket)
{
    QByteArray totalSizeBuffer = socket->read(sizeof(int));
    QDataStream tsDs(totalSizeBuffer);

    int totalSize = 0;
    tsDs >> totalSize;

    // // Create a data stream to read from the socket
    QByteArray buffer;
    QByteArray tmpBuffer;

    while (buffer.size() < totalSize)
    {
        socket->waitForReadyRead();
        tmpBuffer = socket->read(30000);
        buffer.append(tmpBuffer);
    }

    QDataStream ds(buffer);
    socket->close();

    // Read in the size of the Omega Matrix
    int sizeOfOmegaMatrix = 0;
    ds >> sizeOfOmegaMatrix;

    QTextStream(stdout) << "Total Size Recieved " << buffer.size() << "\n";
    QTextStream(stdout) << "Omega Matrix Size " << sizeOfOmegaMatrix << "\n";

    // Create the buffer for the Omega Matrix
    int* omegaMatrix = (int *) malloc(sizeOfOmegaMatrix * sizeof(int));

    // Read in the omega matrix
    for (int i = 0; i < sizeOfOmegaMatrix; i++)
    {
        ds >> omegaMatrix[i];
    }

    // Read in the size of the key list
    int totalAmountOfKeys = 0;
    ds >> totalAmountOfKeys;

    // Read in the size of the key list
    int sizeOfKeyList = 0;
    ds >> sizeOfKeyList;

    // Read in the size of a key
    int sizeOfEachKey = 0;
    ds >> sizeOfEachKey;

    //Read in N
    int n = 0;
    ds >> n;

    // Read in T
    int t = 0;
    ds >> t;

    // Create buffer for key list
    unsigned char* keyList = (unsigned char *)malloc(sizeOfKeyList * sizeOfEachKey);

    // Read in the key list
    for (int i = 0; i < sizeOfKeyList * sizeOfEachKey; i++)
    {
        ds >> keyList[i];
    }

    // Read all servers ip's and ports
    int machineNumber, numberOfAddresses, sizeOfEachAddress;
    ds >> machineNumber;
    ds >> numberOfAddresses;
    ds >> sizeOfEachAddress;

    //Create a list of address/port pairs
    QList<QPair<QString, int>*>* addresses = new QList<QPair<QString, int>*>();
    for (int i = 0; i < numberOfAddresses; i++) 
    {
        QChar ch;
        QString address;
        for (int j = 0; j < sizeOfEachAddress; j++) 
        {
            ds >> ch;
            address.append(ch);
        }
        QStringList l = address.split(QLatin1Char(':'));
        QString ip = l.at(0);
        int port = l.at(1).toInt();

        addresses->append(new QPair<QString, int>(ip, port));
    }

    // Create QMAP that maps a keyID to Key
    QMap<int, unsigned char*>* keyListMap = new QMap<int, unsigned char*>();
    for (int i = 0; i < sizeOfKeyList; i++)
    {
        int keyID = *(omegaMatrix + (i + (machineNumber * sizeOfKeyList)));

        unsigned char* key = (unsigned char *) malloc(sizeOfEachKey);
        memcpy(key, keyList + (i * sizeOfEachKey), sizeOfEachKey);

        keyListMap->insert(keyID, key);
    }

    free(keyList);

    QMap<int, QSet<int>*>* omegaMap = new QMap<int, QSet<int>*>();
    
    // n rows in the omega matrix
    for (int i = 0; i < n; i++)
    {
        QSet<int>* omegaRow = new QSet<int>();
        // int* omegaRow = (int *) malloc(sizeOfKeyList * sizeof(int));
        // std::memcpy(omegaRow, omegaMatrix + (sizeOfKeyList * i), sizeOfKeyList * sizeof(int));
        for (int j = 0; j < sizeOfKeyList; j++)
        {
            int keyId = *(omegaMatrix + ((sizeOfKeyList * i) + j));
            omegaRow->insert(keyId);
        }
        
        omegaMap->insert(i, omegaRow);
    }

    free(omegaMatrix);

    // Save references into enviorment
    environment->set_total_key_num(totalAmountOfKeys);
    environment->set_machine_num(machineNumber);
    environment->set_keys_per_machine(sizeOfKeyList);
    environment->set_size_of_each_key(sizeOfEachKey);
    environment->set_ref_to_addresses(addresses);
    environment->set_ref_to_key_list(keyListMap);
    environment->set_ref_to_omega_table(omegaMap);
    environment->set_N(n);
    environment->set_T(t);
    environment->print_environment();

    QTextStream(stdout) << "Dealer Transaction Complete" << "\n";
}

void DISEServer::handleClient(QTcpSocket* socket)
{
    QByteArray totalSizeBuffer = socket->read(sizeof(int));
    QDataStream tsDs(totalSizeBuffer);

    int totalSize = 0;
    tsDs >> totalSize;

    // Create a data stream to read from the socket
    QByteArray buffer;
    QByteArray tmpBuffer;

    while (buffer.size() < totalSize)
    {
        socket->waitForReadyRead(1000);
        tmpBuffer = socket->read(30000);
        buffer.append(tmpBuffer);
    }

    QDataStream ds(buffer);

    // Get encryption mode
    int encMode = 0;
    ds >> encMode;

    // Get the message
    int sizeOfMessage = 0;
    ds >> sizeOfMessage;

    QString messageString;
    unsigned char* message = (unsigned char *)malloc(sizeOfMessage);
    for (int i = 0; i < sizeOfMessage; i++)
    {
        QChar ch;
        ds >> ch;
        messageString.append(ch);
        message[i] = ch.toLatin1();
    }

    if (encMode == ENCRYPTION)
    {
        QTextStream(stdout) << "Encrypting this message: " << messageString << "\n";

        handleEncryptionRequest(socket, message, sizeOfMessage);

    }
    else
    {
        QTextStream(stdout) << "Decrypting this message: " << messageString << "\n";
        
    }

    ///////////////////////////////////////////////////////////////////

    QTextStream(stdout) << "Honest Initiator xoring results and checking robustness" << "\n";
    
    
    // TODO 
    // temp remove and replace with actual result or turn result into a QString
    QString finalResult = "Temp return result";

    // ///////////////////////////////////////////////////////////////////

    // Build return message
    QTextStream(stdout) << "Honest Initiator Sending Back Result" << "\n";
    QByteArray writeBuffer;
    QDataStream out(&writeBuffer, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    int robustFlag = 0;
    // Robust flag 0 all servers honest 1 there was a dishonest partipant
    out << uint32_t(robustFlag);

    // Write size of enc/dec message and the enc/dec message
    out << uint32_t(finalResult.size());
    const QChar* messageToWrite = finalResult.data();
    for (int i = 0; i < finalResult.size(); i++)
    {
        out << messageToWrite[i];
    }
    
    // Write Total Size
    QByteArray totalWriteSize;
    QDataStream outSize(&totalWriteSize, QIODevice::WriteOnly);
    outSize.setVersion(QDataStream::Qt_4_5);

    outSize << writeBuffer.size();

    socket->write(totalWriteSize);
    socket->flush();
    socket->waitForBytesWritten(1000);

    // Write Return Message
    char* data = writeBuffer.data();

    int maxWrite = 30000;

    int bytesWritten = 0;
    while (bytesWritten < writeBuffer.size())
    {
        // if we have less bytes remaining to write than the max size
        if (writeBuffer.size()- bytesWritten < maxWrite)
        {
            maxWrite = writeBuffer.size() - bytesWritten;
        }

        // write to client
        bytesWritten += socket->write(data + bytesWritten, maxWrite);
        socket->flush();
        socket->waitForBytesWritten(1000);
    }

    QTextStream(stdout) << "Wrote: " << bytesWritten << " to Client" << "\n";

    socket->close();
    QTextStream(stdout) << "Client Transaction Complete" << "\n";

    // Free memory
    // TODO
    // Need to do this just about every where
    // either calling delete, clear for simple maps,lists,sets, or qDeleteAll, clear for complex qdata types
    //qDeleteAll( *partialResultsMap );  //  deletes all the values stored in "map"
    //partialResultsMap->clear();        //  removes all items from the map
}

void DISEServer::handleEncryptionRequest(QTcpSocket* socket, unsigned char* message, int sizeOfMessage)
{
    // Declare the (m || p) and (a = h(m || p))
    unsigned char* mess_cat_p = (unsigned char *) malloc(sizeOfMessage + sizeof(long));
    unsigned char* a_cat_j = (unsigned char *) malloc(A_BYTE_SIZE + sizeof(int));
    
    // Make a random number P
    unsigned char p[sizeof(long)];
    RAND_bytes(p, sizeof(long));

    // Concat the mesage and long random number p
    memcpy(mess_cat_p, message, sizeOfMessage);
    memcpy(mess_cat_p + sizeOfMessage, p, sizeof(long));

    // hash (m||p) to obtain a
    cryptoHash(mess_cat_p, sizeOfMessage + sizeof(long), a_cat_j);

    // validate that the given a is the same as the new a
    std::cout << "og a" << std::endl;
    for (int i = 0; i < A_BYTE_SIZE; i++) {
        std::cout << a_cat_j[i] << " ";
    }
    std::cout << std::endl;

    // Concat a with j the machine number to get (a||j)
    int j = environment->get_machine_num();
    memcpy(a_cat_j + A_BYTE_SIZE, (void *) &j, sizeof(int));

    ////////////////////////////////////////////////////////////////////////////////

    // get paritipant servers
    QList<int>* participantServers = getParticipantServerList();

    // Decide what keys will be used by each server
    QMap<int, QList<int>*>* serverKeysToUse = getParticipantServerKeyMap(participantServers);

    // Set up shared partial results int keyId char result and the robust flag
    QMap<int, unsigned char*>* partialResultsMap = new QMap<int, unsigned char*>();
    bool robustFlag = true;
    
    QTextStream(stdout) << "Honest Initiator Creating Threads" << "\n";
    
    // Start all the threads to ask the partipant servers for their partial results
    // this also performs the robust step
    std::vector<std::thread> threadVector;
    for (int i = 0; i < participantServers->size(); i++)
    {
        int serverId = participantServers->at(i);
        QString ip = environment->get_ref_to_addresses()->value(serverId)->first;
        int port = environment->get_ref_to_addresses()->value(serverId)->second;
        QList<int>* keysToUse = serverKeysToUse->value(serverId);
        threadVector.emplace_back(&DISEServer::honestInitiatorThread, this, ip, port, keysToUse, a_cat_j, partialResultsMap, &robustFlag); 
    }

    // Encrypt the honest initiator machines keys
    QList<int>* honestKeysToUse = serverKeysToUse->value(environment->get_machine_num());
    QMap<int, unsigned char*>* honestPartialResults = encryptDecryptWithKeys(honestKeysToUse, a_cat_j, A_BYTE_SIZE + sizeof(int), ENCRYPTION);

    // set up w 
    unsigned char* w = (unsigned char *) malloc(A_BYTE_SIZE + sizeof(int));
    memset(w, 0x0, A_BYTE_SIZE + sizeof(int));

    // xor honest init results into w
    QMap<int, unsigned char*>::iterator honestResultIter;
    for (honestResultIter = honestPartialResults->begin(); honestResultIter != honestPartialResults->end(); ++honestResultIter)
    {
        unsigned char* curMessage = honestResultIter.value();

        for (long unsigned int i = 0; i < A_BYTE_SIZE + sizeof(int); i++)
        {
            w[i] = w[i] ^ curMessage[i];
        }
    }

    // Join Threads
    QTextStream(stdout) << "Honest Initiator Joining Threads" << "\n";
    for(auto& t: threadVector)
    {
        t.join();
    }

    QTextStream(stdout) << "Threads Joined" << "\n";
    // Free memory
    threadVector.clear();

    QTextStream(stdout) << "Partial results" << "\n";
    QTextStream(stdout) << "Size " << partialResultsMap->size() << "\n";
    QTextStream(stdout) << "Robust Flag " << robustFlag << "\n";

    if (robustFlag)
    { 
        // No partipating server was compromised finish encryptions

        // Join the other servers results into the final w
        QMap<int, unsigned char*>::iterator participantResultIter;
        for (participantResultIter = partialResultsMap->begin(); participantResultIter != partialResultsMap->end(); ++participantResultIter)
        {
            unsigned char* curMessage = participantResultIter.value();

            for (long unsigned int i = 0; i < A_BYTE_SIZE + sizeof(int); i++)
            {
                w[i] = w[i] ^ curMessage[i];
            }
        }

        // Generate Random PRGW
        unsigned char* prgw = (unsigned char * ) malloc(sizeOfMessage + sizeof(long));
        randomNumberWithSeed(w, A_BYTE_SIZE + sizeof(int), prgw, sizeOfMessage + sizeof(long));

        // XOR PRG(w) with (m || p)
        unsigned char* cipherText = (unsigned char * ) malloc(sizeOfMessage + sizeof(long));
        for (long unsigned int i = 0; i < sizeOfMessage + sizeof(long); i++)
        {
            cipherText[i] = prgw[i] ^ mess_cat_p[i];
        }

        // Return result to client
        handleDecryptionRequest(socket, cipherText, sizeOfMessage + sizeof(long), a_cat_j);
    }
    else
    {
        // Write out compromised server message
    }
}

void DISEServer::handleDecryptionRequest(QTcpSocket* socket, unsigned char* cipherText, int cipherTextSize, unsigned char* a_cat_j)
{
    // get paripant servers
    QList<int>* participantServers = getParticipantServerList();

    // Decide what keys will be used by each server
    QMap<int, QList<int>*>* serverKeysToUse = getParticipantServerKeyMap(participantServers);

    // Set up shared partial results int keyId QByteArray encryption result
    QMap<int, unsigned char*>* partialResultsMap = new QMap<int, unsigned char*>();
    bool robustFlag = true;
    
    QTextStream(stdout) << "Honest Initiator Creating Threads" << "\n";
    
    std::vector<std::thread> threadVector;
    for (int i = 0; i < participantServers->size(); i++)
    {
        int serverId = participantServers->at(i);
        QString ip = environment->get_ref_to_addresses()->value(serverId)->first;
        int port = environment->get_ref_to_addresses()->value(serverId)->second;
        QList<int>* keysToUse = serverKeysToUse->value(serverId);
        threadVector.emplace_back(&DISEServer::honestInitiatorThread, this, ip, port, keysToUse, a_cat_j, partialResultsMap, &robustFlag);
    }

    // Encrypt the honest init keys
    QList<int>* honestKeysToUse = serverKeysToUse->value(environment->get_machine_num());
    QMap<int, unsigned char*>* honestPartialResults = encryptDecryptWithKeys(honestKeysToUse, a_cat_j, A_BYTE_SIZE + sizeof(int), ENCRYPTION);

    // set up w 
    unsigned char* w = (unsigned char *) malloc(A_BYTE_SIZE + sizeof(int));
    memset(w, 0x0, A_BYTE_SIZE + sizeof(int));

    // xor honest init results
    QMap<int, unsigned char*>::iterator honestResultIter;
    for (honestResultIter = honestPartialResults->begin(); honestResultIter != honestPartialResults->end(); ++honestResultIter)
    {
        unsigned char* curMessage = honestResultIter.value();

        for (long unsigned int i = 0; i < A_BYTE_SIZE + sizeof(int); i++)
        {
            w[i] = w[i] ^ curMessage[i];
        }
    }

    // Join Threads
    QTextStream(stdout) << "Honest Initiator Joining Threads" << "\n";
    for(auto& t: threadVector)
    {
        t.join();
    }

    QTextStream(stdout) << "Threads Joined" << "\n";
    // Free memory
    threadVector.clear();

    // Check if the threads found a non redundent partial w between the other partipant servers
    if (!robustFlag)
    {
        // early termination there was a redundancy issue
        return;
    }
        
    // Join the other servers results into the final w
    QMap<int, unsigned char*>::iterator participantResultIter;
    for (participantResultIter = partialResultsMap->begin(); participantResultIter != partialResultsMap->end(); ++participantResultIter)
    {
        unsigned char* curMessage = participantResultIter.value();

        for (long unsigned int i = 0; i < A_BYTE_SIZE + sizeof(int); i++)
        {
            w[i] = w[i] ^ curMessage[i];
        }
    }

    // Generate Random PRGW
    unsigned char* prgw = (unsigned char * ) malloc(cipherTextSize);
    randomNumberWithSeed(w, A_BYTE_SIZE + sizeof(int), prgw, cipherTextSize);

    // XOR PRG(w) with (c1) => m||p
    unsigned char* plainText = (unsigned char * ) malloc(cipherTextSize);
    for (int i = 0; i < cipherTextSize; i++)
    {
        plainText[i] = prgw[i] ^ cipherText[i];
    }

    // Check that a is the same as h(m || p)
    unsigned char* newA = (unsigned char *) malloc(A_BYTE_SIZE);
    cryptoHash(plainText, cipherTextSize, newA);
    
    // validate that the given a is the same as the new a
    for (int i = 0; i < A_BYTE_SIZE && robustFlag; i++) {
        if (a_cat_j[i] != newA[i])
        {
            std::cout << "Hash doesn't match" << std::endl;
            robustFlag = false;
        }
    }

    // check robust again after hash check write out either resulting plain text or failure message
    if (robustFlag)
    {
        // send back to client message and robust flag
        QTextStream(stdout) << "Resulting Plain Text" << "\n";
        for (long unsigned int i = 0; i < cipherTextSize - sizeof(long); i++) {
            std::cout << plainText[i];
        }
        std::cout << std::endl;
        QTextStream(stdout) << "From:" << "\n";
        for (long unsigned int i = 0; i < cipherTextSize - sizeof(long); i++) {
            std::cout << cipherText[i];
        }
        std::cout << std::endl;
    }
    else 
    {
        // write out failure
    }

}

void DISEServer::honestInitiatorThread(QString ip, int port, QList<int>* keysToUse, unsigned char* a_cat_j, QMap<int, unsigned char*>* partialResults, bool* robustFlag)
{
    QTextStream(stdout) << "thread going" << "\n";
    
    // Connect to partipant server
    QTcpSocket *socket = new QTcpSocket();
    socket->connectToHost(ip, port);

    if(socket->waitForConnected(3000))
    {
        QTextStream(stdout) << "Thread Connected \n";

        socket->write("2"); // Participant 

        // Create a data stream to the socket
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);

        // Write amount of keys 
        out << uint32_t(keysToUse->size());

        // Write keyIds
        for (int i = 0; i < keysToUse->size(); i++)
        {
            out << uint32_t(keysToUse->at(i));
        }

        // Write a_cat_j
        for (long unsigned int i = 0; i < A_BYTE_SIZE + sizeof(int); i++)
        {
            out << a_cat_j[i];
        }

        // Write total size of the message
        QByteArray totalSize;
        QDataStream outSize(&totalSize, QIODevice::WriteOnly);
        outSize.setVersion(QDataStream::Qt_4_5);

        outSize << block.size();
        socket->write(totalSize);
        socket->flush();
        socket->waitForBytesWritten(1000);

        // Write the full message buffered
        char* data = block.data();

        int maxWrite = 30000;

        int bytesWritten = 0;
        while (bytesWritten < block.size())
        {
            // if we have less bytes remaining to write than the max size
            if (block.size()- bytesWritten < maxWrite)
            {
                maxWrite = block.size() - bytesWritten;
            }

            // write to server
            bytesWritten += socket->write(data + bytesWritten, maxWrite);
            socket->flush();
            socket->waitForBytesWritten(1000);
        }

        QTextStream(stdout) << "Wrote: " << QString::number(bytesWritten) + " to Participant Server" << "\n";

        ///////////////////////////////////////////////////////////////////

        // Wait for results from DISE Servers
        socket->waitForReadyRead();
        QByteArray totalSizeBuffer = socket->read(sizeof(int));
        QDataStream tsDs(totalSizeBuffer);

        int totalReadSize = 0;
        tsDs >> totalReadSize;

        // Create a data stream to read from the socket
        QByteArray resultsBuffer;
        QByteArray tmpBuffer;

        while (resultsBuffer.size() < totalReadSize)
        {
            socket->waitForReadyRead();
            tmpBuffer = socket->read(30000);
            resultsBuffer.append(tmpBuffer);
        }

        QDataStream ds(resultsBuffer);

        socket->close();

        QMap<int, unsigned char*>* myPartialResults = new QMap<int, unsigned char*>();
        for (int i = 0; i < keysToUse->size(); i++)
        {
            int keyId;
            unsigned char* value = (unsigned char * ) malloc(A_BYTE_SIZE + sizeof(int));

            ds >> keyId;
            for (long unsigned int j = 0; j < A_BYTE_SIZE + sizeof(int); j++)
            {
                ds >> value[j];
            }

            myPartialResults->insert(keyId, value);
        }

        mtx->lock();
        // Do the robust check and add to total results
        QMap<int, unsigned char*>::iterator resultsIter;
        for (resultsIter = myPartialResults->begin(); resultsIter != myPartialResults->end() && *robustFlag; ++resultsIter)
        {
            int keyId = resultsIter.key();

            if (partialResults->contains(keyId))
            {
                // Check robustness
                if (memcmp(partialResults->value(keyId), resultsIter.value(), A_BYTE_SIZE + sizeof(int)) != 0) 
                {
                    *robustFlag = false;
                    QTextStream(stdout) << "Not Robust, Triggered by key: " << keyId << "\n";
                }
            }
            else
            {
                partialResults->insert(keyId, resultsIter.value());
            }
        }
        mtx->unlock();

    }
    else
    {
        QTextStream(stdout) << "Not connected" << "\n";
    }

    QTextStream(stdout) << "Thread for server: " << ip << " " << port << " complete" << "\n";
}

QMap<int, unsigned char*>* DISEServer::encryptDecryptWithKeys(QList<int>* keyList, unsigned char* message, int msgSize, int mode)
{
    QMap<int, unsigned char*>* partialResults = new QMap<int, unsigned char*>();

    // for each key in the keys to use list
    for (int i = 0; i < keyList->size(); i++)
    {
        unsigned char* key = environment->get_ref_to_key_list()->value(keyList->at(i));
        // where the encrypted or decrypted msg will be saved
        unsigned char* resultMessage = (unsigned char *)malloc(MAX_CIPHER_LEN);
        switch(mode)
        {
            case 0: // ENCRYPT
                encrypt(message, msgSize, key, resultMessage);
                break;
            case 1: // DECRYPT
                decrypt(message, msgSize, key, resultMessage);
                break;
        }
        
        partialResults->insert(keyList->at(i), resultMessage);
    }

    return partialResults;
}

int DISEServer::encrypt(unsigned char* message, int msgLen, unsigned char* key, unsigned char* encryptedMessage) 
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

int DISEServer::decrypt(unsigned char* message, int msgLen, unsigned char* key, unsigned char* decryptedMessage) 
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

unsigned int DISEServer::cryptoHash(unsigned char* data, int dataLen, unsigned char* result) 
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

void DISEServer::randomNumberWithSeed(unsigned char* seed, int seedLen, unsigned char* result, int resultSize) 
{
    // RAND_seed(seed, seedLen);
    // RAND_bytes(result, resultSize);
    for (int i = 0; i < resultSize; i++) 
    {
        result[i] = seed[i % seedLen] * 2;
    }

    // mbedtls_hmac_drbg_context *ctx;
    // mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

    // mbedtls_hmac_drbg_init(  ctx );

	// mbedtls_hmac_drbg_init(ctx);
    // mbedtls_hmac_drbg_seed_buf(ctx, mbedtls_md_info_from_type(md_type), seed, seedLen);
}

void DISEServer::handleHonestInitiator(QTcpSocket* socket)
{

    QByteArray totalSizeBuffer = socket->read(sizeof(int));
    QDataStream tsDs(totalSizeBuffer);

    int totalSize = 0;
    tsDs >> totalSize;

    // Create a data stream to read from the socket
    QByteArray buffer;
    QByteArray tmpBuffer;

    while (buffer.size() < totalSize)
    {
        socket->waitForReadyRead(1000);
        tmpBuffer = socket->read(30000);
        buffer.append(tmpBuffer);
    }

    QDataStream ds(buffer);

    // Get the amount of keys
    int amountOfKeys = 0;
    ds >> amountOfKeys;

    // Read the key ids into a List
    QList<int>* keyList = new QList<int>();
    for (int i = 0; i < amountOfKeys; i++)
    {
        int keyId;
        ds >> keyId;
        keyList->append(keyId);
    }

    unsigned char* a_cat_j = (unsigned char *)malloc(A_BYTE_SIZE + sizeof(int));
    for (long unsigned int i = 0; i < A_BYTE_SIZE + sizeof(int); i++)
    {
        unsigned char ch;
        ds >> ch;
        a_cat_j[i] = ch;
    }

    QMap<int, unsigned char*>* partialResults = encryptDecryptWithKeys(keyList, a_cat_j, A_BYTE_SIZE + sizeof(int), ENCRYPTION);

    // Create a data stream to the socket
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    // Write key followed by the resulting ciphertext
    QMap<int, unsigned char*>::iterator resultIter;
    for (resultIter = partialResults->begin(); resultIter != partialResults->end(); ++ resultIter)
    {
        out << uint32_t(resultIter.key());

        // the resulting cipherText for the key
        for (long unsigned int i = 0; i < A_BYTE_SIZE + sizeof(int); i++)
        {
            out << resultIter.value()[i];
        }
    }

    // Write total size of the message
    QByteArray totalSizeOut;
    QDataStream outSize(&totalSizeOut, QIODevice::WriteOnly);
       
    outSize.setVersion(QDataStream::Qt_4_5);

    outSize << block.size();
    socket->write(totalSizeOut);
    socket->flush();
    socket->waitForBytesWritten(1000);

    int maxWrite = 30000;
    int bytesWritten = 0;
    char* data = block.data();
    while (bytesWritten < block.size())
    {
        // if we have less bytes remaining to write than the max size
        if (block.size()- bytesWritten < maxWrite)
        {
            maxWrite = block.size() - bytesWritten;
        }

        // write to server
        bytesWritten += socket->write(data + bytesWritten, maxWrite);
        socket->flush();
        socket->waitForBytesWritten(1000);
    }
    socket->close();

    QTextStream(stdout) << "Wrote: " << QString::number(bytesWritten) + " to Honest Initiator Server" << "\n";


    QTextStream(stdout) << "Honest Initiator Transaction Complete" << "\n";
}

QList<int>* DISEServer::getParticipantServerList()
{
    // Random combination of t other participant servers
    QList<int>* partipantServerNumbers = new QList<int>();
    // Populate list of server options
    QList<int>* serverOptions = new QList<int>();
    for (int i = 0; i < environment->get_N(); i++)
    {
        // contains every server excluding the honest initiator
        if (i != environment->get_machine_num())
        {
            serverOptions->append(i);
        }
    }

    // select participants randomly
    srand(time(NULL));
    for (int i = 0; i < environment->get_T(); i++)
    {
        int partipantIndex = rand() % serverOptions->size();
        partipantServerNumbers->append(serverOptions->at(partipantIndex));
        serverOptions->removeAt(partipantIndex);
    }

    // Free memory
    delete serverOptions;

    return partipantServerNumbers;
}

void DISEServer::printServerKeysToUse(QMap<int, QList<int>*>* serverKeysToUse)
{
    // iterate through n machines held keys
    QMap<int, QList<int>*>::iterator serverIter;
    for (serverIter = serverKeysToUse->begin(); serverIter != serverKeysToUse->end(); ++serverIter)
    {
        std::cout << "\t" << serverIter.key() << ": ";

        // print key values in omega row
        QList<int>* keysToUse = serverIter.value();

        for (int i = 0; i < keysToUse->size(); i++)
            std::cout << keysToUse->at(i) << " ";
        
        std::cout << "\n";
    }
}

QMap<int, QList<int>*>* DISEServer::getParticipantServerKeyMap(QList<int>* participantServers)
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