#ifndef DEALER_SERVER_H
#define DEALER_SERVER_H
    #include "DealerDriver.h"

    class DealerServer : public QObject
    {
        Q_OBJECT
        public:
            explicit DealerServer(int port, bool debug = false, QObject *parent = nullptr);
            ~DealerServer();

        Q_SIGNALS:
            void newMessage(QString);

        private Q_SLOTS:
            void newConnection();
            void appendToSocketList(QTcpSocket* socket);
            void readSocket();
            void discardSocket();
        
        private:
            QTcpServer* m_server;
            QSet<QTcpSocket*> connection_set;
            bool debug;
            int port;
    };      
#endif