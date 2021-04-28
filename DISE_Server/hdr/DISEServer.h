/*************************************************************
 * Header file detailing the Dise Server Object
 * 
 * @file DISEServer.h
 * @author Alexander Castro, Garrett Christian,
 *         Rachel Litscher , Steve Chang
 * 
 *************************************************************/ 

#ifndef DISE_SERVER_H
#define DISE_SERVER_H
    #include "DISEServerDriver.h"
    #include "Environment.h"

    /**********************************************************
     * Encapsulation of a DISEServer. It implements the 
     * functionality of the DISE Server, as well as the honest
     * initiator and participating servers.
     **********************************************************/ 
    class DISEServer : public QObject
    {
        Q_OBJECT
        public:
            /**************************************************
             * A Constructor.
             * 
             * Constructs the DISEServer object by instantiating
             * each of the private member attributes.
             * 
             * @param port is the port for the server binding
             * @param debug  is the debug mode flag
             * @param env is the environment reference for the server
             * @param parent is the parent of the QT class
             *************************************************/ 
            explicit DISEServer(int port, bool debug = false, Environment* env = nullptr, QObject *parent = nullptr);
            
            /**************************************************
             * A Destructor.
             * 
             * Destructs the DISEServer object by freeing the 
             * underlying memory contained in the member
             * attributes
             *************************************************/
            ~DISEServer();

        Q_SIGNALS:

        private Q_SLOTS:
            /*****************************************************
             * QT Action functions that are triggered with the 
             * socket event occurs.
             ****************************************************/
            void newConnection();
            void appendToSocketList(QTcpSocket* socket);
            void readSocket();
            void discardSocket();

            /*****************************************************
             * Handles a dealer request by recieving all of the 
             * associcated dealer information and storing it in the
             * enviorment reference.
             * 
             * @param socket is the current socket being processed
             ****************************************************/
            void handleDealer(QTcpSocket* socket);

            /*****************************************************
             * Handles a client requests and processes what type
             * of transaction is being requested. With call another
             * function depending on the type of transaction.
             * 
             * @param socket is the current socket being processed
             ****************************************************/
            void handleClient(QTcpSocket* socket);

            /*****************************************************
             * Represents the honest initiatior role and does all 
             * of the necessary work to communicate with the 
             * participating servers to complete whichever transaction
             * type is requested.
             * 
             * @param socket is the current socket being processed
             ****************************************************/
            void handleHonestInitiator(QTcpSocket* socket);

            /*****************************************************
             * Initiates a encryption transaction for the given
             * message of messageSize.
             * 
             * @param socket is the current socket being processed
             * @param message is the plaintext that will be encrypted
             * @param sizeOfMessage is the size of the message to be 
             *                      encrypted
             ****************************************************/
            void handleEncryptionRequest(QTcpSocket* socket, unsigned char* message, int sizeOfMessage);

            /*****************************************************
             * Initiates a decryption transaction for the given
             * ciphertext of cipherTextSize with the calculated
             * A concatenated with J.
             * 
             * @param socket is the current socket being processed
             * @param cipherText is the cipherText that will be
             *                   decrypted
             * @param cipherTextSize is the size of the given 
             *                       cipherText
             * @param a_cat_j is the value of A concatenated with J
             ****************************************************/
            void handleDecryptionRequest(QTcpSocket* socket, unsigned char* cipherText, int cipherTextSize, unsigned char* a_cat_j);

            /*****************************************************
             * Thread function that the honest initator uses to 
             * communicate with the participating servers. T count
             * of these will be spawned
             * 
             * @param ip is the IP address of the participating server
             * @param port is the port of the participating server
             * @param keysToUse are the keyIds that are involved in
             *                  the transaction
             * @param a_cat_j is the value of A concatenated with J
             * @param paritalResults is the map of paritalResults
             * @param robustFlag is the flag to tell is the scheme 
             *                   is robust
             ****************************************************/
            void honestInitiatorThread(QString ip, int port, QList<int>* keysToUse, unsigned char* a_cat_j, QMap<int, unsigned char*>* partialResults, bool* robustFlag);

        private:
            QTcpServer* m_server;             // QTServer Object for receiveing connections
            QSet<QTcpSocket*> connection_set; // Set of connection requests 
            bool debug;                       // Flag that dictates debug output
            int port;                         // Server port number
            Environment* environment;         // Reference to the Enviorment of the server 
            std::mutex* mtx;                  // Mutex lock
    };      
#endif