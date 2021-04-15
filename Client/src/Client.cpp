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
        QTextStream(stdout) << "connecting..." << "\n";

    socket->connectToHost(ip, port);

    if(socket->waitForConnected(3000))
    {
        QTextStream(stdout) << "Connected to " << ip << port << "\n";

        socket->write("1"); // Client

        // Create a data stream to the socket
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);

        if (encMode == 1)
        {
            QTextStream(stdout) << "Decrypting this message: " << message << "\n";
        }
        else
        {
            QTextStream(stdout) << "Encrypting this message: " << message << "\n";
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

        QTextStream(stdout) << "Wrote: " << QString::number(bytesWritten) + " to Honest Initiator" << "\n";

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
            QTextStream(stdout) << "All partipant servers returned the correct results" << "\n";
        }
        else
        {
            QTextStream(stdout) << "There was a compromised partipant server in the transaction" << "\n";
        }

        QTextStream(stdout) << "Resulting message: " << returnMessage << "\n";

    }
    else
    {
        QTextStream(stdout) << "Not connected" << "\n";
    }
}

void Client::connected()
{
    if (debug)
        QTextStream(stdout) << "connected..." << "\n";
}

void Client::disconnected()
{
    if (debug)
        QTextStream(stdout) << "disconnected..." << "\n";
}

void Client::bytesWritten(qint64 bytes)
{
    if (debug)
        QTextStream(stdout) << bytes << " bytes written..." << "\n";
}

void Client::readyRead()
{
    if (debug)
        QTextStream(stdout) << "reading..." << "\n";
}