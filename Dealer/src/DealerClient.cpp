#include "../hdr/DealerClient.h"

DealerClient::DealerClient(QObject *parent) :
    QObject(parent)
{
}

DealerClient::~DealerClient()
{
}

void DealerClient::doConnect(KeyGenerator* key_gen, QString ip, int port, int machine_num)
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

        int* omega_matrix = key_gen->get_ref_to_omega_matrix();

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);


        out << uint32_t(key_gen->get_size_of_omega_table()); // number of elements, allows for 4 billion elements
        
        for (int i=0; i < key_gen->get_size_of_omega_table(); i++)
            out << omega_matrix[i];
        
        auto key_list = key_gen->get_ref_to_key_list();

        out << uint32_t(key_gen->get_key_count_per_machine());
        out << uint32_t(key_gen->get_size_of_each_key());

        for (int i = 0; i < key_gen->get_key_count_per_machine(); i++)
        {
            int key_index = omega_matrix[i + (machine_num * key_gen->get_key_count_per_machine())];
            unsigned char* key = key_list->at(key_index);

            for (int j=0; j < key_gen->get_size_of_each_key(); j++)
                out << key[j];
        }


        socket->write(block);
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