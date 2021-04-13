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

    std::cout << "Total Size: " << totalSize << std::endl;

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

    qDebug() << "Dealer Message Recieved";
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

    // Create buffer for key list
    unsigned char* keyList = (unsigned char *)malloc(sizeOfKeyList * sizeOfEachKey);

    // Read in the key list
    for (int i = 0; i < sizeOfKeyList * sizeOfEachKey; i++)
    {
        ds >> keyList[i];
    }

    // Read all servers ip's and ports
    int machineNumber = 0;
    int numberOfAddresses = 0;
    int sizeOfEachAddress = 0;
    ds >> machineNumber;
    ds >> numberOfAddresses;
    ds >> sizeOfEachAddress;
    QList<std::tuple<QString, int>> addresses = QList<std::tuple<QString, int>>();

    qDebug() << "Machine Number " << machineNumber;
    qDebug() << "# addresses " << numberOfAddresses << " size of each " << sizeOfEachAddress;

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

        qDebug() << i << " " << ip << " " << port;

        addresses.append(std::make_tuple(ip, port));
    }

    if (debug)
    {

        qDebug() << "Size of Key List: " << sizeOfKeyList;
        qDebug() << "Size of Each Key: " << sizeOfEachKey;  

        // Print out the List of Keys assigned to this machine
        for (int i = 0; i < sizeOfKeyList; i++)
        {
            std::cout << *(omegaMatrix + (i + (machineNumber * sizeOfKeyList))) + 1 << ":  "; 
            for (int j = 0; j < sizeOfEachKey; j++)
            {
                printf("%X ", keyList[j + (sizeOfEachKey * i)]);
            }
            std::cout << "\n";
        }
    }

    qDebug() << "Dealer Transaction Complete";

    // socket->write("Dealer Transaction Complete");
    // socket->flush();
    // socket->waitForBytesWritten(5000);

}
