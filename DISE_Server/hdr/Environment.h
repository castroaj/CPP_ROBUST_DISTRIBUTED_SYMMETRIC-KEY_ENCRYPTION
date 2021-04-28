/*************************************************************
 * Header file detailing the Enviorment Object
 * 
 * @file Environment.h
 * @author Alexander Castro, Garrett Christian,
 *         Rachel Litscher , Steve Chang
 * 
 *************************************************************/ 
#ifndef ENVIRONMENT_H
    #define ENVIRONMENT_H
    #include "DISEServerDriver.h"
    
    /*********************************************************
     * Encapsulates all of the necessary enviorment variables
     * needed for the Dealer to operate and connect to active
     * DISE Servers.
     *********************************************************/ 
    class Environment
    {
        private:
            unsigned int threadCount; // Number of threads OpenMP will use
            int machineNum;           // Number associated with the current DISE Server
            int keysPerMachine;       // Number of keys per DISE Server
            int sizeOfEachKey;        // Size of a single Key
            int totalKeyNum;          // Total Number of keys in Scheme
            int N;                    // Total number of DISE Servers
            int T;                    // Threshold count for a single transaction
            bool compromised;         // Flag indicating if the server is compromised

            QList<QPair<QString, int>*>* addresses; // Structure that holds the addresses and ports
            QMap<int, QSet<int>*>* omegaTable;      // Structure that holds the omega matrix
            QMap<int, unsigned char*>* keyList;     // Structure that holds all of the keys

        public:
            /**************************************************
             * A Constructor.
             * 
             * Constructs the Environment object by instantiating
             * each of the private member attributes.
             **************************************************/
            Environment();

            /**************************************************
             * A Destructor.
             * 
             * Destructs the Environment object by freeing the 
             * underlying memory contained in the member
             * attributes
             *************************************************/
            ~Environment();

            /*************************************************
             * Function that outputs all of the data contained
             * within the Environment instance
             ************************************************/
            void print_environment();

            /************************************************
             * Helper Function that returns whether a given
             * server has a given keyId
             * 
             * @param server is the serverID
             * @param keyId is the KeyID
             * @return whether or not the key is assigned to
             *         the given server
             ***********************************************/ 
            bool server_owns_key(int server, int keyId);

            /*************************************************
             * Getter and Setter for Thread Count
             ************************************************/
            unsigned int get_thread_count() { return threadCount; }
            void set_thread_count(unsigned int tc) { threadCount = tc; }

            /*************************************************
             * Getter and Setter for the Machine Number
             ************************************************/
            int get_machine_num() { return machineNum; }
            void set_machine_num(int mn) { machineNum = mn; }

            /*************************************************
             * Getter and Setter for Total Key Number
             ************************************************/
            int get_total_key_num() { return totalKeyNum; }
            void set_total_key_num(int tkn) { totalKeyNum = tkn; }

            /*************************************************
             * Getter and Setter for Key count per machine
             ************************************************/
            int get_keys_per_machine() { return keysPerMachine; }
            void set_keys_per_machine(int kpm) { keysPerMachine = kpm; }

            /*************************************************
             * Getter and Setter for Key Size
             ************************************************/
            int get_size_of_each_key() { return sizeOfEachKey; }
            void set_size_of_each_key(int size) { sizeOfEachKey = size; }

            /*************************************************
             * Getter and Setter for N
             ************************************************/
            int get_N() { return N; }
            void set_N(int n) { N = n; }

            /*************************************************
             * Getter and Setter for T
             ************************************************/
            int get_T() { return T; }
            void set_T(int t) { T = t; }

            /*************************************************
             * Getter and Setter for Address List
             ************************************************/
            QList<QPair<QString, int>*>* get_ref_to_addresses() { return addresses; }
            void set_ref_to_addresses(QList<QPair<QString, int>*>* a) { addresses = a; }

            /*************************************************
             * Getter and Setter for Omega Table
             ************************************************/
            QMap<int, QSet<int>*>* get_ref_to_omega_table() { return omegaTable; }
            void set_ref_to_omega_table(QMap<int, QSet<int>*>* ot) { omegaTable = ot; }

            /*************************************************
             * Getter and Setter for Key List
             ************************************************/
            QMap<int, unsigned char*>* get_ref_to_key_list() { return keyList; }
            void set_ref_to_key_list(QMap<int, unsigned char*>* kl) { keyList = kl; }

            /*************************************************
             * Getter and Setter for Comprimised Flag
             ************************************************/
            bool is_compromised() { return compromised; }
            void set_compromised(bool comp) { compromised = comp; }

    };

#endif