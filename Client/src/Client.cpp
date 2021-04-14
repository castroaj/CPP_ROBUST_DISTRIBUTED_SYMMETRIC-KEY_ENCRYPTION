#include "../hdr/Client.h"

Client::Client(bool d, QObject *parent) :
    QObject(parent)
{
    debug = d;
}

Client::~Client()
{
}

void Client::doConnect(QString ip, int port, unsigned int encMode, QString message)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    if (debug)
        qDebug() << "connecting...";

    socket->connectToHost(ip, port);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected to " << ip << port;

        socket->write("1"); // Client

        // Create a data stream to the socket
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);

        if (encMode == 1)
        {
            qDebug() << "Decrypting this message: " << message;
        }
        else
        {
            qDebug() << "Encrypting this message: " << message;
        }

        // Write encryption mode 
        out << uint32_t(encMode);

        // Write size of message
        out << message.size(); 
        const QChar* messageToWrite = message.data();
        for (int i = 0; i < message.size(); i++)
        {
            out << messageToWrite[i];
        }

        // Write total size of the message
        QByteArray totalSize;
        QDataStream outSize(&totalSize, QIODevice::WriteOnly);
        outSize.setVersion(QDataStream::Qt_4_5);

        outSize << block.size();
        socket->write(totalSize);
        socket->flush();
        socket->waitForBytesWritten(1000);

        // Write the full message buffered
        char* data = block.data();

        int maxWrite = 30000;

        int bytesWritten = 0;
        while (bytesWritten < block.size())
        {
            // if we have less bytes remaining to write than the max size
            if (block.size()- bytesWritten < maxWrite)
            {
                maxWrite = block.size() - bytesWritten;
            }

            // write to server
            bytesWritten += socket->write(data + bytesWritten, maxWrite);
            socket->flush();
            socket->waitForBytesWritten(1000);
        }

        qDebug() << "Wrote: " << QString::number(bytesWritten) + " to Honest Initiator";

        ///////////////////////////////////////////////////////////////////

        // Wait for results from DISE Servers
        socket->waitForReadyRead();
        QByteArray totalSizeBuffer = socket->read(sizeof(int));
        QDataStream tsDs(totalSizeBuffer);

        int totalReadSize = 0;
        tsDs >> totalReadSize;

        // Create a data stream to read from the socket
        QByteArray resultsBuffer;
        QByteArray tmpBuffer;

        while (resultsBuffer.size() < totalReadSize)
        {
            socket->waitForReadyRead();
            tmpBuffer = socket->read(30000);
            resultsBuffer.append(tmpBuffer);
        }

        QDataStream ds(resultsBuffer);

        socket->close();

        // Flag to see if there was a compromised server
        int robustFlag = 0;
        ds >> robustFlag;

        // Return Message
        int sizeOfReturnMessage = 0;
        ds >> sizeOfReturnMessage;

        QString returnMessage;
        for (int i = 0; i < sizeOfReturnMessage; i++)
        {
            QChar ch;
            ds >> ch;
            returnMessage.append(ch);
        }

        if (robustFlag == 0)
        {
            qDebug() << "All partipant servers returned the correct results";
        }
        else
        {
            qDebug() << "There was a compromised partipant server in the transaction";
        }

        qDebug() << "Resulting message: " << returnMessage;

    }
    else
    {
        qDebug() << "Not connected";
    }
}

void Client::connected()
{
    if (debug)
        qDebug() << "connected...";
}

void Client::disconnected()
{
    if (debug)
        qDebug() << "disconnected...";
}

void Client::bytesWritten(qint64 bytes)
{
    if (debug)
        qDebug() << bytes << " bytes written...";
}

void Client::readyRead()
{
    if (debug)
        qDebug() << "reading...";
}