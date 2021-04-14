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

    // // Create QMAP that maps a keyID to Key
    QMap<int, unsigned char*>* keyListMap = new QMap<int, unsigned char*>();
    for (int i = 0; i < sizeOfKeyList; i++)
    {
        int keyID = *(omegaMatrix + (i + (machineNumber * sizeOfKeyList)));

        unsigned char* key = (unsigned char *) malloc(sizeOfEachKey);
        memcpy(key, keyList + (i * sizeOfEachKey), sizeOfEachKey);

        keyListMap->insert(keyID, key);
    }

    free(keyList);

    QMap<int, int*>* omegaMap = new QMap<int, int*>();
    
    for (int i = 0; i < n; i++)
    {
        int* omegaRow = (int *) malloc(sizeOfKeyList * sizeof(int));
        std::memcpy(omegaRow, omegaMatrix + (sizeOfKeyList * i), sizeOfKeyList * sizeof(int));
        
        omegaMap->insert(i, omegaRow);
    }

    // Save references into enviorment
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

    // Decide combination of t other participant servers
    // Decide what keys will be used by each server
    // Fork or thread communication to other partipants
    // Do the honest init keys
    // Accumulate all partial results
    // robustness check
    // xor all partial results

    // simulate work
    usleep(10);

    ///////////////////////////////////////////////////////////////////

    // Build return message
    QString result = message.toUpper();
    int robustFlag = 0;

    QByteArray writeBuffer;
    QDataStream out(&writeBuffer, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    // Robust flag 0 all servers honest 1 there was a dishonest partipant
    out << uint32_t(robustFlag);

    // Write size of enc/dec message and the enc/dec message
    out << result.size();
    const QChar* messageToWrite = result.data();
    for (int i = 0; i < result.size(); i++)
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
}

void DISEServer::handleHonestInitiator(QTcpSocket* socket)
{

    socket->close();
    qDebug() << "Honest Initiator Transaction Complete";
}
