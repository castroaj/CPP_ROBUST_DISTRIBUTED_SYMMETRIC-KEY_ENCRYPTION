#include "../hdr/DealerClient.h"

DealerClient::DealerClient(bool d, QObject *parent) :
    QObject(parent)
{
    debug = d;
}

DealerClient::~DealerClient()
{
}

void DealerClient::doConnect(KeyGenerator* key_gen, QString ip, int port, int machine_num, QList<QString>* addresses)
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

        // Gather information from key_gen object
        auto omega_matrix = key_gen->get_ref_to_omega_matrix();
        auto key_list = key_gen->get_ref_to_key_list();
        auto omega_table_size = key_gen->get_size_of_omega_table();
        auto total_key_count = key_gen->get_key_count();
        auto key_count_per_machine = key_gen->get_key_count_per_machine();
        auto key_size = key_gen->get_size_of_each_key();
        auto n = key_gen->get_n();
        auto t = key_gen->get_t();

        // Dealer Message
        socket->write("0");

        // Create a data stream to the socket
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);

        ///////////////////////////////////////////////////////////////////

        // Write size of omega_table to socket
        out << uint32_t(omega_table_size);
        
        // Write omega table to socket
        for (int i=0; i < omega_table_size; i++)
            out << omega_matrix[i];
        
        
        ///////////////////////////////////////////////////////////////////

        // Write key count and size to socket
        out << uint32_t(total_key_count);
        out << uint32_t(key_count_per_machine);
        out << uint32_t(key_size);

        // Write N and T to socket
        out << uint32_t(n);
        out << uint32_t(t);

        // Write keys associated with the machine to the socket
        for (int i = 0; i < key_count_per_machine; i++)
        {
            // Get the index of the next key for the current machine
            int key_index = omega_matrix[i + (machine_num * key_count_per_machine)];
            unsigned char* key = key_list->at(key_index);

            for (int j=0; j < key_size; j++)
                out << key[j];
        }

        //////////////////////////////////////////////////////////////////

        // Write ip addresses and ports sepertated by a :
        // machine identifier
        out << uint32_t(machine_num);
        // Amount of addresses
        out << uint32_t(addresses->size());
        // Size of each address
        out << uint32_t(addresses->at(0).size());

        // Write out the addresses
        for (int i = 0; i < addresses->size(); i++)
        {
            const QChar* address = addresses->at(i).data();
            for (int j = 0; j < addresses->at(i).size(); j++)
                out << address[j];
        }

        ///////////////////////////////////////////////////////////////////

        QByteArray totalSize;
        QDataStream outSize(&totalSize, QIODevice::WriteOnly);
        outSize.setVersion(QDataStream::Qt_4_5);

        outSize << block.size();
        socket->write(totalSize);
        socket->flush();
        socket->waitForBytesWritten(1000);

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

        qDebug() << "The number of bytes written is: " << bytesWritten;

        // socket->waitForReadyRead(3000);
        // QString complete = socket->readAll();
        // qDebug() << complete;
        socket->close();
    }
    else
    {
        qDebug() << "Not connected";
    }
}

void DealerClient::connected()
{
    if (debug)
        qDebug() << "connected...";
}

void DealerClient::disconnected()
{
    if (debug)
        qDebug() << "disconnected...";
}

void DealerClient::bytesWritten(qint64 bytes)
{
    if (debug)
        qDebug() << bytes << " bytes written...";
}

void DealerClient::readyRead()
{
    if (debug)
    {
        qDebug() << "reading...";

        // read the data from the socket
        QString s = socket->readAll();
        qDebug() << s;
    }
}