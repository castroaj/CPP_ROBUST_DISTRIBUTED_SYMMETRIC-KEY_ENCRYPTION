#include "../hdr/DISEServer.h"

QT_USE_NAMESPACE

//! [constructor]
DISEServer::DISEServer(int port, bool debug, QObject* parent) : 
    QObject(parent),
    m_server(new QTcpServer),
    debug(debug),
    port(port)
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

    //readSocket(socket);
}

void DISEServer::readSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    
    socket->waitForReadyRead(1000);
    qDebug() << "Reading: " << socket->bytesAvailable();

    QByteArray buffer = socket->readAll();
    QDataStream ds(buffer);
    int sizeOfOmegaMatrix = 0;
    ds >> sizeOfOmegaMatrix;

    int omegaMatrix[sizeOfOmegaMatrix];

    for (int i = 0; i < sizeOfOmegaMatrix; i++)
    {
        ds >> omegaMatrix[i];
    }

    //qDebug() << "Size: " << size;
    // for (int row = 0; row < 5; row++) {
    //   printf("\n");
    //   for (int col = 0; col < 6; col++) {
    //      printf("%d ", *(omegaMatrix + row*6 + col));
    //   }
    // }
    // printf("\n");

    int sizeOfKeyList = 0;
    ds >> sizeOfKeyList;

    int sizeOfEachKey = 0;
    ds >> sizeOfEachKey;

    qDebug() << "Size of Key List: " << sizeOfKeyList;
    qDebug() << "Size of Each Key: " << sizeOfEachKey;

    unsigned char keyList[sizeOfKeyList * sizeOfEachKey];

    for (int i = 0; i < sizeOfKeyList * sizeOfEachKey; i++)
    {
        ds >> keyList[i];
    }

    for (int i = 0; i < sizeOfKeyList; i++)
    {
        for (int j = 0; j < sizeOfEachKey; j++)
        {
            printf("%X", keyList[j + (sizeOfEachKey * i)]);
        }
        std::cout << "\n";
    }



    socket->write("Hello Client");
    socket->flush();

    socket->waitForBytesWritten(5000);

    socket->close();
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