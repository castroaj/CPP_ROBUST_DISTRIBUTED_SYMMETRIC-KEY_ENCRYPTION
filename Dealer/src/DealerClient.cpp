#include "../hdr/DealerClient.h"

DealerClient::DealerClient(QObject *parent) :
    QObject(parent)
{
}

DealerClient::~DealerClient()
{
}

void DealerClient::doConnect(KeyGenerator* key_gen, QList<QString>* addresses)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting...";

    for (unsigned int i = 0; i < addresses->size(); i++)
    {
        QString curString = addresses->at(i);

        qDebug() << curString;
    }


    // this is not blocking call
    socket->connectToHost(addresses->at(0), 1234);

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