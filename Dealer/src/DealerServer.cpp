#include "../hdr/DealerServer.h"

QT_USE_NAMESPACE

//! [constructor]
DealerServer::DealerServer(int port, bool debug, QObject* parent) : 
    QObject(parent),
    m_server(new QTcpServer),
    debug(debug),
    port(port)
{
    if(m_server->listen(QHostAddress::Any, port))
    {
       connect(m_server, &QTcpServer::newConnection, this, &DealerServer::newConnection);
       if (debug)
           std::cout << "Server Listening on port " << port << std::endl;
    }
    else
    {
        std::cout << "Failed to bind to port " << port << std::endl;
        exit(EXIT_FAILURE);
    }
}

DealerServer::~DealerServer()
{
    foreach (QTcpSocket* socket, connection_set)
    {
        socket->close();
        socket->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();
}

void DealerServer::newConnection()
{
    while (m_server->hasPendingConnections())
        appendToSocketList(m_server->nextPendingConnection());
}

void DealerServer::appendToSocketList(QTcpSocket* socket)
{
    connection_set.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &DealerServer::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &DealerServer::discardSocket);
}

void DealerServer::readSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray buffer;

    QDataStream socketStream(socket);

    socketStream.startTransaction();
    socketStream >> buffer;

    if(!socketStream.commitTransaction())
    {
        QString message = QString("%1 :: Waiting for more data to come..").arg(socket->socketDescriptor());
        emit newMessage(message);
        return;
    }
}

void DealerServer::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = connection_set.find(socket);
    if (it != connection_set.end()){
        connection_set.remove(*it);
    }
    
    socket->deleteLater();
}