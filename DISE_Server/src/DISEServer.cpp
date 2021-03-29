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
    
    socket->waitForBytesWritten(5000);
    socket->waitForReadyRead(1000);
    qDebug() << "Reading: " << socket->bytesAvailable();

    qDebug() << socket->readAll();

    socket->flush();
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