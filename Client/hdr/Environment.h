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
    #include "ClientDriver.h"
    
    /*********************************************************
     * Encapsulates all of the necessary enviorment variables
     * needed for the Client to operate and connect to active
     * DISE Servers.
     *********************************************************/ 
    class Environment
    {
        private:
            unsigned int thread_count;      // Number of threads Open MP will use
            unsigned int enc_mode;          // Mode of which the Client will operate
            QList<QString>* ip_addresses;   // List of IP addresses of the DISE Servers
                                            
        
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

            /*************************************************
             * Getter and Setter for Thread Count
             ************************************************/
            unsigned int get_thread_count() { return thread_count; }
            void set_thread_count(unsigned int tc) { thread_count = tc; }

            /*************************************************
             * Getter and Setter for the encryption mode
             ************************************************/
            unsigned int get_enc_mode() { return enc_mode; }
            void set_enc_mode(unsigned int enc) { enc_mode = enc; }

            /*************************************************
             * Getter for the Address List
             ************************************************/
            QList<QString>* get_ref_to_addresses() { return ip_addresses; }
    };

#endif