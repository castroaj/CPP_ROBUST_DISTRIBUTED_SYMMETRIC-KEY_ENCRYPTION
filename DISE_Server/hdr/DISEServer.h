#ifndef DISE_SERVER_H
#define DISE_SERVER_H
    #include "DISEServerDriver.h"
    #include "Environment.h"

    class DISEServer : public QObject
    {
        Q_OBJECT
        public:
            explicit DISEServer(int port, bool debug = false, Environment* env = nullptr, QObject *parent = nullptr);
            ~DISEServer();

        Q_SIGNALS:
            void newMessage(QString);

        private Q_SLOTS:
            void newConnection();
            void appendToSocketList(QTcpSocket* socket);
            void readSocket();
            void discardSocket();
            void handleDealer(QTcpSocket* socket);
            void handleClient(QTcpSocket* socket);
            void handleHonestInitiator(QTcpSocket* socket);
            QList<int>* getParticipantServerList();
            QMap<int, QSet<int>*>* getParticipantServerKeyMap(QList<int>* partipantServers);
        
        private:
            QTcpServer* m_server;
            QSet<QTcpSocket*> connection_set;
            bool debug;
            int port;
            Environment* environment;
    };      
#endif