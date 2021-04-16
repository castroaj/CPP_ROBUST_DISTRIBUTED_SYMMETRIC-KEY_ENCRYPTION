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

    if (encMode == 1)
    {
        QTextStream(stdout) << "Decrypting this message: " << messageString << "\n";
    }
    else
    {
        QTextStream(stdout) << "Encrypting this message: " << messageString << "\n";
    }

    ///////////////////////////////////////////////////////////////////

    // get paripant servers
    QList<int>* participantServers = getParticipantServerList();

    // Decide what keys will be used by each server
    QMap<int, QList<int>*>* serverKeysToUse = getParticipantServerKeyMap(participantServers);

    std::cout << "\tKEYS TO BE USED BY EACH SERVER" << std::endl;
    if (environment->get_N() != 24)
        printServerKeysToUse(serverKeysToUse);
    else
        std::cout << "\tKEYS TO BE USED IS TOO BIG TO PRINT" << std::endl;


    // Set up shared partial results int keyId QByteArray encryption result
    QMap<int, QList<unsigned char*>*>* partialResultsMap = new QMap<int, QList<unsigned char*>*>();
    QTextStream(stdout) << "Honest Initiator Creating Threads" << "\n";
    
    std::vector<std::thread> threadVector;
    for (int i = 0; i < participantServers->size(); i++)
    {
        int serverId = participantServers->at(i);
        QString ip = environment->get_ref_to_addresses()->value(serverId)->first;
        int port = environment->get_ref_to_addresses()->value(serverId)->second;
        QList<int>* keysToUse = serverKeysToUse->value(serverId);
        threadVector.emplace_back(&DISEServer::honestInitiatorThread, this, ip, port, keysToUse, message, encMode, partialResultsMap); 
    }

    // Encrypt the honest init keys
    QList<int>* honestKeysToUse = serverKeysToUse->value(environment->get_machine_num());
    QMap<int, unsigned char*>* honestPartialResults = encryptDecryptWithKeys(honestKeysToUse, message, sizeOfMessage, encMode);

    // Join Threads
    QTextStream(stdout) << "Honest Initiator Joining Threads" << "\n";
    for(auto& t: threadVector)
    {
        t.join();
    }

    QTextStream(stdout) << "Threads Joined" << "\n";
    // Free memory
    threadVector.clear();

    QTextStream(stdout) << "Honest Initiator xoring results and checking robustness" << "\n";
    // xor all partial results to get the final result
    // and set robust flag checking if any of the results were not the same
    unsigned char result[sizeOfMessage];
    memset(result, 0x0, sizeOfMessage);
    int robustFlag = 0;
    // QMap<int, QList<unsigned char*>*>::iterator resultIter;
    // for (resultIter = partialResultsMap->begin(); resultIter != partialResultsMap->end(); ++resultIter)
    // {

        // QList<unsigned char*>* partialResults = resultIter.value();
        // char* msg = message.data();

    //     // int cmp = memcmp(partialResults->at(0), partialResults->at(1), size));
    //     // if (cmp == -1)
    //     // {
    //     //     robustFlag = 1;
    //     // }

        // xor partial and result assign to result
        // TODO
        // do the xor on each char a ^ b read into result
        // issue here is to handle different lengths just an if 
        // and the for loop goes to the longest result or new partial 
        // but what do we do with leftover just save as is?
    // }

    // XOR honest results
    // TODO REMOVE AND REFACTOR
    // should be one loop of honest
    qDebug() << "1!!!!!!!!!!!!!!!!!!!!!!!!!!";
    QMap<int, unsigned char*>::iterator honestResultIter;
    for (honestResultIter = honestPartialResults->begin(); honestResultIter != honestPartialResults->end(); ++honestResultIter)
    {
        unsigned char* curMessage = honestResultIter.value();

        for (int i = 0; i < sizeOfMessage; i++){
            std::cout << curMessage[i];
        }
        std::cout << std::endl;

        for (int i = 0; i < sizeOfMessage; i++)
        {
            result[i] = result[i] ^ curMessage[i];
        }
    }

    qDebug() << "RESULT 1";
    for (int i = 0; i < sizeOfMessage; i++){
        std::cout << result[i];
    }
    std::cout << std::endl;

    // Encrypt the honest init keys
    QList<int>* honestKeysToUse2 = serverKeysToUse->value(environment->get_machine_num());
    QMap<int, unsigned char*>* honestPartialResults2 = encryptDecryptWithKeys(honestKeysToUse, result, sizeOfMessage, 1);

    unsigned char result2[sizeOfMessage];
    memset(result2, 0x0, sizeOfMessage);

    qDebug() << "2!!!!!!!!!!!!!!!!!!!!!!!!!!";
    QMap<int, unsigned char*>::iterator honestResultIter2;
    for (honestResultIter2 = honestPartialResults2->begin(); honestResultIter2 != honestPartialResults2->end(); ++honestResultIter2)
    {
        unsigned char* curMessage = honestResultIter2.value();

        for (int i = 0; i < sizeOfMessage; i++){
            std::cout << curMessage[i];
        }
        std::cout << std::endl;

        for (int i = 0; i < sizeOfMessage; i++)
        {
            result2[i] = result2[i] ^ curMessage[i];
        }
    }

    qDebug() << "RESULT2!!!!!";
    for (int i = 0; i < sizeOfMessage; i++){
        std::cout << result2[i];
    }
    std::cout << std::endl;

    
    // TODO 
    // temp remove and replace with actual result or turn result into a QString
    QString finalResult = "Temp return result";

    // ///////////////////////////////////////////////////////////////////

    // Build return message
    QTextStream(stdout) << "Honest Initiator Sending Back Result" << "\n";
    QByteArray writeBuffer;
    QDataStream out(&writeBuffer, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

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
    qDeleteAll( *partialResultsMap );  //  deletes all the values stored in "map"
    partialResultsMap->clear();        //  removes all items from the map
}

void DISEServer::honestInitiatorThread(QString ip, int port, QList<int>* keysToUse, unsigned char* message, int encMode, QMap<int, QList<unsigned char*>*>* partialResults)
{
    QTextStream(stdout) << "thread going" << "\n";
    // TODO write honest -> partipant thread
    // Mutex untested but should be part of the object will protect partialResults
    // Will write to ip, port -> "2", total msg size, amount of keys to use, each int key to use,
    // size of the message, the message, the int enc mode
    // WaitForReadyRead()
    // Read back the partial results into shared partialResults map

    // socket = new QTcpSocket(this);

    // if (debug)
    //     qDebug() << "connecting...";

    // socket->connectToHost(ip, port);

    // if(socket->waitForConnected(3000))
    // {

    // }
    // else
    // {
    //     qDebug() << "Not connected";
    // }
}

QMap<int, unsigned char*>* DISEServer::encryptDecryptWithKeys(QList<int>* keyList, unsigned char* message, int msgSize, int mode)
{
    QMap<int, unsigned char*>* partialResults = new QMap<int, unsigned char*>();

    // for each key in the keys to use list
    int keySize = environment->get_size_of_each_key();
    for (int i = 0; i < keyList->size(); i++)
    {
        unsigned char* key = environment->get_ref_to_key_list()->value(keyList->at(i));
        // where the encrypted or decrypted msg will be saved
        unsigned char* resultMessage = (unsigned char *)malloc(MAX_CIPHER_LEN);
        int resultMessageSize = 0;
        switch(mode)
        {
            case 0: // ENCRYPT
                resultMessageSize = encrypt(message, msgSize, key, resultMessage);
                break;
            case 1: // DECRYPT
                resultMessageSize = decrypt(message, msgSize, key, resultMessage);
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

void DISEServer::handleHonestInitiator(QTcpSocket* socket)
{
    // TODO
    // recieve from threads which keys mode and message
    // call encryptDecryptWithKeys
    // return resulting map

    socket->close();
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