#include "../hdr/Client.h"

Client::Client(bool d, QObject *parent) :
    QObject(parent)
{
    debug = d;
}

Client::~Client()
{
}

void Client::doConnect(QString ip, int port, unsigned int encMode, unsigned char* message, int msgSize, unsigned char* a_cat_j, int a_cat_j_size)
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

        if (encMode == ENCRYPTION)
        {
            QTextStream(stdout) << "Encrypting this message: " << message << "\n";
        }

        // Write encryption mode 
        out << uint32_t(encMode);

        // Write size of message
        out << msgSize; 
        for (int i = 0; i < msgSize; i++)
        {
            out << message[i];
        }

        // if dec 
        if (encMode == DECRYPTION)
        {
            out << uint32_t(a_cat_j_size); 
            for (int i = 0; i < a_cat_j_size; i++)
            {
                out << a_cat_j[i];
            }
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

        std::cout << "here" << std::endl;

        while (resultsBuffer.size() < totalReadSize)
        {
            socket->waitForReadyRead(1000);
            tmpBuffer = socket->read(30000);
            resultsBuffer.append(tmpBuffer);
        }

        QDataStream ds(resultsBuffer);

        std::cout << "here" << std::endl;

        // all data recieved close the socket
        socket->close();

        // Flag to see if there was a compromised server
        bool robustFlag = false;
        ds >> robustFlag;

        std::cout << robustFlag << std::endl;

        // Three possible: successful enc, successful dec, or compromised server
        if (robustFlag && encMode == ENCRYPTION) {
            std::cout << "Reading Successful Encryption" << std::endl;

            // Return Message
            int sizeOfReturnMessage = 0;
            ds >> sizeOfReturnMessage;

            unsigned char returnMessage[sizeOfReturnMessage];
            for (int i = 0; i < sizeOfReturnMessage; i++)
            {
                ds >> returnMessage[i];
            }

            // Recieve a||j
            int a_cat_j_enc_size = 0;
            ds >> a_cat_j_enc_size;
            unsigned char* a_cat_j_enc = (unsigned char *) malloc(a_cat_j_enc_size);

            for (int i = 0; i < a_cat_j_enc_size; i++)
            {
                ds >> a_cat_j_enc[i];
            }

            // Save results of the encryption to use later
            QFile file("encResult.txt");
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

            QDataStream fds(&file);

            // write message to file
            fds << uint32_t(sizeOfReturnMessage);
            for (int i = 0; i < sizeOfReturnMessage; i++)
            {
                fds << returnMessage[i];
            }
            for (int i = 0; i < a_cat_j_enc_size; i++)
            {
                fds << a_cat_j_enc[i];
            }

        }
        else if (robustFlag && encMode == DECRYPTION)
        {
            std::cout << "Reading Successful Encryption" << std::endl;
            // Return Message
            int sizeOfReturnMessage = 0;
            ds >> sizeOfReturnMessage;

            QString returnMessage;
            for (int i = 0; i < sizeOfReturnMessage; i++)
            {
                unsigned char ch;
                ds >> ch;
                returnMessage.append(QChar(ch));
            }

            QTextStream(stdout) << "Resulting message: " << returnMessage << "\n";
        }
        else
        {
            // There was a compromised server
            QTextStream(stdout) << "Robust Flag caught a compromised server in this transaction" << "\n";
        }

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