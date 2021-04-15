#include "../hdr/DISEServer.h"

QT_USE_NAMESPACE

DISEServer::DISEServer(int port, bool debug, Environment* env, QObject* parent) : 
    QObject(parent),
    m_server(new QTcpServer),
    debug(debug),
    port(port),
    environment(env)
{
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
            qDebug() << "Dealer Message Recieved";
            handleDealer(socket);
            break;
        case 1: // CLIENT MESSAGE
            qDebug() << "Client Message Recieved";
            handleClient(socket);
            break;
        case 2: // HONEST INITIATOR MESSAGE
            qDebug() << "Honest Initiator Message Recieved";
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

    qDebug() << "Total Size Recieved " << buffer.size();
    qDebug() << "Omega Matrix Size " << sizeOfOmegaMatrix;

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

    qDebug() << "Dealer Transaction Complete";
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

    int sizeOfMessage = 0;
    ds >> sizeOfMessage;

    QString message;
    for (int i = 0; i < sizeOfMessage; i++)
    {
        QChar ch;
        ds >> ch;
        message.append(ch);
    }

    if (encMode == 1)
    {
        qDebug() << "Decrypting this message: " << message;
    }
    else
    {
        qDebug() << "Encrypting this message: " << message;
    }

    ///////////////////////////////////////////////////////////////////

    // get paripant servers
    QList<int>* participantServers = getParticipantServerList();

    qDebug() << "Random participants: " << *participantServers;

    // Decide what keys will be used by each server
    QMap<int, QList<int>*>* serverKeysToUse = getParticipantServerKeyMap(participantServers);

    std::cout << "\tKEYS TO BE USED BY EACH SERVER" << std::endl;
    if (environment->get_N() != 24)
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
    else
    {
        std::cout << "\tKEYS TO BE USED IS TOO BIG TO PRINT" << std::endl;
    }

    // Set up shared partial results int keyId QByteArray encryption result
    qDebug() << "Honest Initiator Creating Threads";
    QMap<int, QList<unsigned char*>*>* partialResultsMap = new QMap<int, QList<unsigned char*>*>();
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
    QSet<int>* honestKeys = environment->get_ref_to_omega_table()->value(environment->get_machine_num());
    QSet<int>::iterator honestKeyIter;

    qDebug() << "Honest Initiator Encrypting";
    for (honestKeyIter = honestKeys->begin(); honestKeyIter != honestKeys->end(); ++honestKeyIter)
    {
        // TODO
        // call encrprypt with each key held by the honest init
        // *honestKeyIter is the key to be passed
    }

    // Join Threads
    qDebug() << "Honest Initiator Joining Threads";
    for(auto& t: threadVector)
    {
        t.join();
    }
    qDebug() << "Threads Joined";
    // Free memory
    threadVector.clear();

    qDebug() << "Honest Initiator xoring results and checking robustness";
    // xor all partial results to get the final result
    // and set robust flag checking if any of the results were not the same
    unsigned char* result;
    int robustFlag = 0;
    // QMap<int, QList<unsigned char*>*>::iterator resultIter;
    // for (resultIter = partialResultsMap->begin(); resultIter != partialResultsMap->end(); ++resultIter)
    // {
        // TODO
        // issue here need to convert the map to either
        // QMap<int, QList<QPair<unsigned char*, int>*>*>*
        // keyId -> results for that key -> plainText, plainText size
        // OR
        // QMap<int, QList<QString>*>*
        // and just send strings back
        // idk this bit is very dependant on what the threads send and how enc / dec will work

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
    
    // TODO 
    // temp remove and replace with actual result or turn result into a QString
    QString finalResult = "Temp return result";

    // ///////////////////////////////////////////////////////////////////

    // Build return message
    qDebug() << "Honest Initiator Sending Back Result";
    QByteArray writeBuffer;
    QDataStream out(&writeBuffer, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    // Robust flag 0 all servers honest 1 there was a dishonest partipant
    out << uint32_t(robustFlag);

    // Write size of enc/dec message and the enc/dec message
    out << uint32_t(finalResult.size());
    const QChar* messageToWrite = finalResult.data();
    for (uint32_t i = 0; i < finalResult.size(); i++)
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

    qDebug() << "Wrote: " << bytesWritten << " to Client";

    socket->close();
    qDebug() << "Client Transaction Complete";

    // Free memory
    // TODO
    // Need to do this just about every where
    // either calling delete, clear for simple maps,lists,sets, or qDeleteAll, clear for complex qdata types
    qDeleteAll( *partialResultsMap );  //  deletes all the values stored in "map"
    partialResultsMap->clear();        //  removes all items from the map
}

void DISEServer::honestInitiatorThread(QString ip, int port, QList<int>* keysToUse, QString message, int encMode, QMap<int, QList<unsigned char*>*>* partialResults)
{
    qDebug() << "thread going";
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

int DISEServer::encrypt(unsigned char* message, int msgLen, int keyId, int keySize, unsigned char* encryptedMessage) {
    unsigned char* key = environment->get_ref_to_key_list()->value(keyId);
    unsigned char iv[keySize];

    // https://github.com/saju/misc/blob/master/misc/openssl_aes.c
    // and pa2 from 457

    // // no salt used
    // int keySize = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), NULL, key, keySize, 14, iv);

    // int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
    // int status ;
    // unsigned len=0 , encryptedLen=0 ;

    // /* Create and initialise the context */
    // EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new() ;
    // if( ! ctx )
    //     handleErrors("encrypt: failed to creat CTX");

    // // Initialise the encryption operation.
    // status = EVP_EncryptInit_ex( ctx, ALGORITHM(), NULL, key, iv ) ;
    // if( status != 1 )
    //     handleErrors("encrypt: failed to EncryptInit_ex");

    // // Call EncryptUpdate as many times as needed (e.g. inside a loop)
    // // to perform regular encryption
    // status = EVP_EncryptUpdate(ctx, pCipherText, &len, pPlainText, plainText_len) ;
    // if( status != 1 )
    //     handleErrors("encrypt: failed to EncryptUpdate");
    // encryptedLen += len;

    // // If additional ciphertext may still be generated,
    // // the pCipherText pointer must be first advanced forward
    // pCipherText += len ;

    // // Finalize the encryption.
    // status = EVP_EncryptFinal_ex( ctx, pCipherText , &len ) ;
    // if( status != 1 )
    //     handleErrors("encrypt: failed to EncryptFinal_ex");
    // encryptedLen += len; // len could be 0 if no additional cipher text was generated

    // /* Clean up */
    // EVP_CIPHER_CTX_free(ctx);

    return 0;
}

int DISEServer::decrypt(unsigned char* message, int msgLen, int keyId, int keySize, unsigned char* decryptedMessage) {

    // unsigned char* key = environment->get_ref_to_key_list()->value(keyId);
    // unsigned char iv[keySize];
    // unsigned len = 0; 
    // unsigned decryptedLen = 0;

    // /* Create and initialise the context */
    // EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new() ;

    // // Initialise the decryption operation.
    // status = EVP_DecryptInit_ex( ctx, EVP_aes_256_cbc(, NULL, key, iv ) ;
    // if( status != 1 )
    //     handleErrors("decrypt: failed to DecryptInit_ex");

    // // Call DecryptUpdate as many times as needed (e.g. inside a loop)
    // // to perform regular decryption
    // status = EVP_DecryptUpdate( ctx, pDecryptedText, &len, pCipherText, cipherText_len) ;
    // if( status != 1 )
    //     handleErrors("decrypt: failed to DecryptUpdate");
    // decryptedLen += len;

    // // If additionl decrypted text may still be generated,
    // // the pDecryptedText pointer must be first advanced forward
    // pDecryptedText += len ;

    // // Finalize the decryption.
    // status = EVP_DecryptFinal_ex( ctx, pDecryptedText , &len ) ;
    // if( status != 1 )
    //     handleErrors("decrypt: failed to DecryptFinal_ex");
    // decryptedLen += len;

    // /* Clean up */
    // EVP_CIPHER_CTX_free(ctx);

    // return decryptedLen;

    return 0;
}

void DISEServer::handleHonestInitiator(QTcpSocket* socket)
{
    // TODO

    socket->close();
    qDebug() << "Honest Initiator Transaction Complete";
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
    }

    // Free memory
    delete numberOfKeysAssigned;

    return serverKeysToUse;
}