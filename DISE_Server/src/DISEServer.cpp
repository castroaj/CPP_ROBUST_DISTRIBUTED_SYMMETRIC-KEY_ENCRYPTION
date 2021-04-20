#include "../hdr/DISEServer.h"
#include "../hdr/DISEServerUtil.h"

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

    unsigned char* message = (unsigned char *)malloc(sizeOfMessage);
    for (int i = 0; i < sizeOfMessage; i++)
    {
        ds >> message[i];
    }

    // if decryption a_cat_j will also be sent
    int a_cat_j_size = 0;
    ds >> a_cat_j_size;

    unsigned char* a_cat_j = (unsigned char *)malloc(a_cat_j_size);
    for (int i = 0; i < a_cat_j_size; i++)
    {
        ds >> a_cat_j[i];
    }

    // Pass the socket along to the corrisponding operation
    if (encMode == ENCRYPTION)
    {
        QTextStream(stdout) << "Encrypting" << "\n";
        handleEncryptionRequest(socket, message, sizeOfMessage);
    }
    else
    {
        QTextStream(stdout) << "Decrypting" << "\n";
        handleDecryptionRequest(socket, message, sizeOfMessage, a_cat_j);
    }

    // Clean up socket
    socket->close();
    QTextStream(stdout) << "Client Transaction Complete" << "\n";

    // Free memory
    free(message);
    free(a_cat_j);
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
    QList<int>* participantServers = getParticipantServerList(environment->get_N(), environment->get_T(), environment->get_machine_num());

    // Decide what keys will be used by each server
    QMap<int, QList<int>*>* serverKeysToUse = getParticipantServerKeyMap(participantServers, environment);

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
    QMap<int, unsigned char*>* honestPartialResults = encryptWithKeyList(honestKeysToUse, a_cat_j, A_BYTE_SIZE + sizeof(int), environment->get_ref_to_key_list());

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

    // block to write data to
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    // Write total size of the message
    QByteArray totalSize;
    QDataStream outSize(&totalSize, QIODevice::WriteOnly);
    outSize.setVersion(QDataStream::Qt_4_5);

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

        // Free the prgw memory
        free(prgw);

        // Return result to client
        std::cout << "Encryption successful writing to client" << std::endl;
        // write out cipher text
        out << bool(robustFlag);
        out << uint32_t(sizeOfMessage + sizeof(long));
        for (long unsigned int i = 0 ; i < sizeOfMessage + sizeof(long); i++)
        {
            out << cipherText[i];
        }
        // write out a||j
        out << uint32_t(A_BYTE_SIZE + sizeof(int));
        for (long unsigned int i = 0; i < sizeOfMessage + sizeof(long); i++)
        {
            out << a_cat_j[i];
        }
        outSize << block.size();
        socket->write(totalSize);
        socket->flush();
        socket->waitForBytesWritten(1000);
        socket->write(block);
        socket->flush();
        socket->waitForBytesWritten(1000);

        
        free(cipherText);
    }
    else
    {
        // Write out compromised server message
        out << robustFlag;
        outSize << uint32_t(sizeof(int));
        socket->write(totalSize);
        socket->flush();
        socket->waitForBytesWritten(1000);
        socket->write(block);
        socket->flush();
        socket->waitForBytesWritten(1000);
    }

    free(a_cat_j);
    free(w);
    free(mess_cat_p);

    delete partialResultsMap;
    delete participantServers;
    serverKeysToUse->clear();
    delete serverKeysToUse;
}

void DISEServer::handleDecryptionRequest(QTcpSocket* socket, unsigned char* cipherText, int cipherTextSize, unsigned char* a_cat_j)
{
    // get participant servers
    QList<int>* participantServers = getParticipantServerList(environment->get_N(), environment->get_T(), environment->get_machine_num());

    // Decide what keys will be used by each server
    QMap<int, QList<int>*>* serverKeysToUse = getParticipantServerKeyMap(participantServers, environment);

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
    QMap<int, unsigned char*>* honestPartialResults = encryptWithKeyList(honestKeysToUse, a_cat_j, A_BYTE_SIZE + sizeof(int), environment->get_ref_to_key_list());

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

    // block to write data to
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    // Write total size of the message
    QByteArray totalSize;
    QDataStream outSize(&totalSize, QIODevice::WriteOnly);
    outSize.setVersion(QDataStream::Qt_4_5);

    // Check if the threads found a non redundent partial w between the other partipant servers
    if (!robustFlag)
    {
        // Early termination there was a redundancy issue
        // Write out compromised server message
        out << robustFlag;
        outSize << uint32_t(sizeof(int));
        socket->write(totalSize);
        socket->flush();
        socket->waitForBytesWritten(1000);
        socket->write(block);
        socket->flush();
        socket->waitForBytesWritten(1000);
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
        // Write robust flag
        out << robustFlag;

        // Write client message
        int plainTextSize = cipherTextSize - sizeof(long);
        out << plainTextSize;
        QTextStream(stdout) << "Resulting Plain Text" << "\n";
        for (int i = 0; i < plainTextSize; i++) {
            std::cout << plainText[i];
            out << plainText[i];
        }
        std::cout << std::endl;

        // Write total size
        outSize << block.size();
        socket->write(totalSize);
        socket->flush();
        socket->waitForBytesWritten(1000);
        socket->write(block);
        socket->flush();
        socket->waitForBytesWritten(1000);
    }
    else 
    {
        // Write out compromised server message, hash was not good
        out << robustFlag;
        outSize << uint32_t(sizeof(int));
        socket->write(totalSize);
        socket->flush();
        socket->waitForBytesWritten(1000);
        socket->write(block);
        socket->flush();
        socket->waitForBytesWritten(1000);
    }

    free(w);
    free(newA);
    free(prgw);
    free(plainText);

    delete participantServers;
    delete partialResultsMap;

    serverKeysToUse->clear();
    delete serverKeysToUse;

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
        delete socket;

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

    QMap<int, unsigned char*>* partialResults = encryptWithKeyList(keyList, a_cat_j, A_BYTE_SIZE + sizeof(int), environment->get_ref_to_key_list());

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

    QMap<int, unsigned char*>::iterator freeIter;
    for (freeIter = partialResults->begin(); freeIter != partialResults->end(); ++ freeIter)
    {
        free(resultIter.value());
    }
    delete partialResults;

}