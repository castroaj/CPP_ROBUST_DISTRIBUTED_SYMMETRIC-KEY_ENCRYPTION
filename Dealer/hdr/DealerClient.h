#ifndef DEALER_CLIENT_H
#define DEALER_CLIENT_H
    #include "DealerDriver.h"
    #include "KeyGenerator.h"

    class DealerClient : public QObject
    {
        Q_OBJECT
        public:
            explicit DealerClient(QObject *parent = nullptr);
            ~DealerClient();
            void doConnect(KeyGenerator* key_gen, QList<QString>* addresses);
            
        Q_SIGNALS:
            //void sendMesssage();

        private Q_SLOTS:
            void connected();
            void disconnected();
            void bytesWritten(qint64 bytes);
            void readyRead();

        private:
            QTcpSocket *socket;
            bool debug;
            int port;
    };      
#endif