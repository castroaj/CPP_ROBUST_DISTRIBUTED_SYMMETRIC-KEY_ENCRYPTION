#include "../hdr/DealerServer.h"

QT_USE_NAMESPACE

//! [constructor]
DealerServer::DealerServer(int port, bool debug, QObject* parent) : 
    QObject(parent),
    p_web_socket_server(new QWebSocketServer(QStringLiteral("Dealer Server"), QWebSocketServer::NonSecureMode, this)),
    debug(debug)
{
    if (p_web_socket_server->listen(QHostAddress::Any, port))
    {
        if (debug)
            std::cout << "Dealer Server listening on port: " << port;

        connect(p_web_socket_server, &QWebSocketServer::newConnection, this, &DealerServer::onNewConnection);
        connect(p_web_socket_server, &QWebSocketServer::closed, this, &DealerServer::closed);
    }
}

DealerServer::~DealerServer()
{
    p_web_socket_server->close();
    qDeleteAll(clients.begin(), clients.end());
}

void DealerServer::onNewConnection()
{
    QWebSocket *pSocket = p_web_socket_server->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &DealerServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &DealerServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &DealerServer::socketDisconnected);
}

void DealerServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (debug)
        std::cout << "Message received: " << message.toLocal8Bit().data() << std::endl;
    
    if (pClient)
        pClient->sendTextMessage(message);
}

void DealerServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (debug)
        std::cout << "Binary Message received" << std::endl;
    
    if (pClient)
        pClient->sendBinaryMessage(message);

}

void DealerServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (debug)
        std::cout << "Socket Disconnected" << std::endl;
    
    if (pClient)
    {
        clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
