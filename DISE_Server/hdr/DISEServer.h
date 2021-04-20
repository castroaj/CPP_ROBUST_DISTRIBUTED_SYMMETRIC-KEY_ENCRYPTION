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
            void handleEncryptionRequest(QTcpSocket* socket, unsigned char* message, int sizeOfMessage);
            void handleDecryptionRequest(QTcpSocket* socket, unsigned char* cipherText, int cipherTextSize, unsigned char* a_cat_j);
            void honestInitiatorThread(QString ip, int port, QList<int>* keysToUse, unsigned char* a_cat_j, QMap<int, unsigned char*>* partialResults, bool* robustFlag);

        private:
            QTcpServer* m_server;
            QSet<QTcpSocket*> connection_set;
            bool debug;
            int port;
            Environment* environment;
            std::mutex* mtx;
    };      
#endif