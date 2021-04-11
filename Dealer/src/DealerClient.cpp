#include "../hdr/DealerClient.h"

DealerClient::DealerClient(QObject *parent) :
    QObject(parent)
{
}

DealerClient::~DealerClient()
{
}

void DealerClient::doConnect(KeyGenerator* key_gen, QString ip, int port)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting...";

    socket->connectToHost(ip, port);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected!";

        // send
        socket->write("hello server\r\n\r\n\r\n\r\n");
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);

        socket->close();
    }
    else
    {
        qDebug() << "Not connected";
    }
}

void DealerClient::connected()
{
    qDebug() << "connected...";

}

void DealerClient::disconnected()
{
    qDebug() << "disconnected...";
}

void DealerClient::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void DealerClient::readyRead()
{
    qDebug() << "reading...";

    // read the data from the socket
    qDebug() << socket->readAll();
}