#ifndef DEALER_CLIENT_H
#define DEALER_CLIENT_H
    #include "DealerDriver.h"
    #include "KeyGenerator.h"

    class DealerClient : public QObject
    {
        Q_OBJECT
        public:
            explicit DealerClient(bool debug, QObject *parent = nullptr);
            ~DealerClient();
            void doConnect(KeyGenerator* key_gen, QString ip, int port, int machine_num, QList<QString>* addresses);
            
        Q_SIGNALS:

        private Q_SLOTS:
            void connected();
            void disconnected();
            void bytesWritten(qint64 bytes);
            void readyRead();

        private:
            QTcpSocket *socket;
            bool debug;
            int port;
            qint64 totalBytesWritten;
    };      
#endif