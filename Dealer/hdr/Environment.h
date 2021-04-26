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
    #include "DealerDriver.h"

    /*********************************************************
     * Encapsulates all of the necessary enviorment variables
     * needed for the Dealer to operate and connect to active
     * DISE Servers.
     *********************************************************/ 
    class Environment
    {
        private:
            unsigned int thread_count;     // Number of threads Open MP will use
            unsigned int dist_mode;        // Mode of which the Dealer will operate
            QList<QString>* ip_addresses;  // List of IP addresses of the DISE Servers
        
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
             * Getter and Setter for the Distribution mode
             ************************************************/
            unsigned int get_dist_mode() { return dist_mode; }
            void set_dist_mode(unsigned int dm) { dist_mode = dm; }

            /*************************************************
             * Getter for the Address List
             ************************************************/
            QList<QString>* get_ref_to_addresses() { return ip_addresses; }
    };

#endif