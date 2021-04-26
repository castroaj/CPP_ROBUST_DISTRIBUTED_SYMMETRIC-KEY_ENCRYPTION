/*************************************************************
 * Header file detailing the DealerClient Object
 * 
 * @file DealerClient.h
 * @author Alexander Castro, Garrett Christian,
 *         Rachel Litscher , Steve Chang
 * 
 *************************************************************/

#ifndef DEALER_CLIENT_H
#define DEALER_CLIENT_H
    #include "DealerDriver.h"
    #include "KeyGenerator.h"

    /**********************************************************
     * Encapsulation of a DealerClient. Implements the connection
     * to a procedure in order to communicate with a DISE 
     * Server
     **********************************************************/ 
    class DealerClient : public QObject
    {
        Q_OBJECT
        public:
            /**************************************************
             * A Constructor.
             * 
             * Constructs the DealerClient object by instantiating
             * each of the private member attributes.
             * 
             * @param debug  is the debug mode flag
             * @param parent is the parent of the QT class
             *************************************************/ 
            explicit DealerClient(bool debug, QObject *parent = nullptr);

            /**************************************************
             * A Destructor.
             * 
             * Destructs the Client object by freeing the 
             * underlying memory contained in the member
             * attributes
             *************************************************/
            ~DealerClient();

            /*************************************************
             * Function that implements the connection procedure
             * with a DISE Server. Called from the DealerDriver
             * application class, and operates in both ENC and DEC
             * Modes.
             * 
             * @param key_gen is a reference to the key generator
             *                object created in the Driver
             * 
             * @param ip is the IP address of the DISE Server to 
             *           connect to. 
             * 
             * @param port is the port associated with the above IP
             *             address.
             * 
             * @param machineNum is the number associated with the
             *                   DISE Server that is being connected
             *                   to. Will dictate which keys are sent.
             * 
             * @param addresses is the list of addresses that are sent to
             *                  DISE Server
             ************************************************/
            void doConnect(KeyGenerator* key_gen, QString ip, int port, 
                           int machine_num, QList<QString>* addresses);
            
        Q_SIGNALS:

        private Q_SLOTS:

            /*****************************************************
             * QT Action functions that are triggered with the 
             * socket event occurs.
             ****************************************************/
            void connected();
            void disconnected();
            void bytesWritten(qint64 bytes);
            void readyRead();

        private:
            QTcpSocket *socket;       // QT Socket object that is used to
                                      // Connect to a DISE Server
            bool debug;               // Debug Flag for additional output
            int port;                 // Port that the socket will connect to
            qint64 totalBytesWritten; // Value that tracks number of bytes written
    };      
#endif