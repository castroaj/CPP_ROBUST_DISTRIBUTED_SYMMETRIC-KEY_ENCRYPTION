#ifndef CLIENT_H
#define CLIENT_H
    #include "ClientDriver.h"

    class Client : public QObject
    {
        Q_OBJECT
        public:
            explicit Client(bool debug, QObject *parent = nullptr);
            ~Client();
            void doConnect(QString ip, int port, unsigned int encMode, unsigned char* message, int msgSize, unsigned char* a_cat_j, int a_cat_j_size);
            
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