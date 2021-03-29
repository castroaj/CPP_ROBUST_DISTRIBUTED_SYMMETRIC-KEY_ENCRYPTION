#ifndef DEALER_CLIENT_H
#define DEALER_CLIENT_H
    #include "DealerDriver.h"

    class DealerClient : public QObject
    {
        Q_OBJECT
        public:
            explicit DealerClient(QObject *parent = nullptr);
            ~DealerClient();
            void doConnect();
            
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