#include "../hdr/DealerClient.h"

DealerClient::DealerClient(QObject *parent) :
    QObject(parent)
{
}

DealerClient::~DealerClient()
{
}

void DealerClient::doConnect()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting...";

    // this is not blocking call
    socket->connectToHost("localhost", 1234);

    socket->write("Hi Server");

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();

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