#ifndef DEALER_SERVER_H
#define DEALER_SERVER_H
    #include "DealerDriver.h"

    QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
    QT_FORWARD_DECLARE_CLASS(QWebSocket)

    class DealerServer : public QObject
    {
        Q_OBJECT
        public:
            explicit DealerServer(int port, bool debug = false, QObject *parent = nullptr);
            ~DealerServer();

        Q_SIGNALS:
            void closed();

        private Q_SLOTS:
            void onNewConnection();
            void processTextMessage(QString message);
            void processBinaryMessage(QByteArray message);
            void socketDisconnected();
        
        private:
            QWebSocketServer *p_web_socket_server;
            QList<QWebSocket *> clients;
            bool debug;
    };      


#endif