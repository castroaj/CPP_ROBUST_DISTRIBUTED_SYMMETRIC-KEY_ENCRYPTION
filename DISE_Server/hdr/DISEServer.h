#ifndef DISE_SERVER_H
#define DISE_SERVER_H
    #include "DISEServerDriver.h"

    class DISEServer : public QObject
    {
        Q_OBJECT
        public:
            explicit DISEServer(int port, bool debug = false, QObject *parent = nullptr);
            ~DISEServer();

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