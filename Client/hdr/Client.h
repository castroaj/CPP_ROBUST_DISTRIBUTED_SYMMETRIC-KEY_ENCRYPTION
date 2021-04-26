/*************************************************************
 * Header file detailing the Client Object
 * 
 * @file Client.h
 * @author Alexander Castro, Garrett Christian,
 *         Rachel Litscher , Steve Chang
 * 
 *************************************************************/ 

#ifndef CLIENT_H
#define CLIENT_H
    #include "ClientDriver.h"

    /**********************************************************
     * Encapsulation of a Client. Implements the connection
     * to a procedure in order to communicate with a DISE 
     * Server
     **********************************************************/ 
    class Client : public QObject
    {
        Q_OBJECT
        public:
            /**************************************************
             * A Constructor.
             * 
             * Constructs the Client object by instantiating
             * each of the private member attributes.
             * 
             * @param debug  is the debug mode flag
             * @param parent is the parent of the QT class
             *************************************************/ 
            explicit Client(bool debug, QObject *parent = nullptr);

            /**************************************************
             * A Destructor.
             * 
             * Destructs the Client object by freeing the 
             * underlying memory contained in the member
             * attributes
             *************************************************/
            ~Client();

            /*************************************************
             * Function that implements the connection procedure
             * with a DISE Server. Called from the ClientDriver
             * application class, and operates in both ENC and DEC
             * Modes.
             * 
             * @param ip is the IP Address of the selected DISE Server.
             *           This server will be label the honest initator
             *           throughout the rest of the DISE Transaction
             * 
             * @param port is the port assoicated with the above IP adress.
             *             This will be used when creating a socket
             *             connection with the DISE Server
             * 
             * @param encMode is the flag of which operation will occur
             *                during the current transaction
             * 
             * @param message is either the plaintext or the ciphertext
             *                depending on the mode of operation
             * 
             * @param msgSize is an integer indicating the size
             *                of the input message
             * 
             * @param a_cat_j is a decryption specific piece of 
             *                information that is used within the 
             *                cryptographic scheme to get restore 
             *                the plaintext
             * 
             * @param a_cat_j_size is the size of the previously defined
             *                     message
             * 
             *****************************************************/ 
            void doConnect(QString ip, int port, unsigned int encMode, 
                           unsigned char* message, int msgSize,
                           unsigned char* a_cat_j, int a_cat_j_size);
            
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
            QTcpSocket *socket;  // QT Socket object that is used to
                                 // Connect to a DISE Server
            bool debug;          // Debug Flag used for additional output
            int port;            // Port that the socket will connect to
    };      
#endif