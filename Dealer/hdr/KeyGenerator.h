/*************************************************************
 * Header file detailing the KeyGenerator Object
 * 
 * @file KeyGenerator.h
 * @author Alexander Castro, Garrett Christian,
 *         Rachel Litscher , Steve Chang
 * 
 *************************************************************/ 

#ifndef KEY_GEN_H
    #define KEY_GEN_H
    #include "DealerDriver.h"

    /**********************************************************
     * Encapsulation of a KeyGenerator object. Used to create 
     * necessary cryptographic data structures for each of the
     * DISE Servers. 
     **********************************************************/ 
    class KeyGenerator
    {
        private:
            int key_count;                       // Total Number of Keys in Scheme
            int key_count_per_machine;           // Number of keys assigned to each machine
            int key_size;                        // Number of bytes in a particular key
            int n;                               // Total Number of machines is DISE Scheme 
            int t;                               // Number of machines required for a single transaction
            int key_idx;                         // Index into the list of keys
            int* omega_col_idxs;                 // Indicates for each row in omega, which col to put a key
            int* omega_matrix;                   // 2D Array that holds the end result of the Omega Matrix
            QList<unsigned char*>* all_key_list; // List of all keys generated


            /**************************************************
             * Function that uses OpenSSL's RAND_BYTES function
             * to create key_count number random keys of size 
             * key_size. Use OpenMP to paralellize this process  
             **************************************************/ 
            void create_all_key_list();

            /*************************************************
             * Function takes an array of server numbers
             * (corresponding to the row numbers in the omega 
             * table) and the index of the array of keys. Takes
             * the key at that index and populates that all servers
             * in the omega table with that key id. 
             * 
             * @param servers is the list of servers
             * @param key_idx is the index into the list of keys 
             *************************************************/
            void populate_omega_table(int servers[], int key_idx);

            /*******************************************************
             * Recursive Helper function that finds all the combos
             * of 't' servers, given a total of 'n' servers. Once
             * a combo is found, it passes the combo of servers and
             * the index into the array of keys, to a function to
             * populat the omega matrix properly.
             * 
             * @param server_combo is the temp array to hold the
             *                     combo of servers
             * 
             * @param start_num is the starting index into key_ids
             * 
             * @param combo_idx is the current index into server_combo
             * 
             * @param id_idx is the index into the key_ids to know 
             *               what id the server gets
             *************************************************/
            void find_combos(int server_combo[], int start_num, int combo_idx);

            /*****************************************************
             * Helper function that initiates the creation of 
             * the omega matrix data structure.
             *****************************************************/
            void generate_omega_matrix();

            /******************************************************
             * Helper function that dumps the omega table to the 
             * console.
             ******************************************************/ 
            void print_omega_table();

            /******************************************************
             * Implements a combonation calculator with the
             * representation of n over k
             * 
             * @param n is the upper value in the combonation
             * @param k is the lower value in the combonation
             *****************************************************/
            unsigned nChoosek(unsigned n, unsigned k);


        public:
            /**************************************************
             * A Constructor.
             * 
             * Constructs the KeyGenerator object by instantiating
             * each of the private member attributes.
             * 
             * @param dist_mode  is the dist_mode flag
             *************************************************/ 
            KeyGenerator(unsigned int dist_mode);

            /**************************************************
             * A Destructor.
             * 
             * Destructs the KeyGenerator object by freeing the 
             * underlying memory contained in the member
             * attributes
             *************************************************/
            ~KeyGenerator();

            /*************************************************
             * Helper Function that dumps the attributes of 
             * the KeyGenerator Object to the console.
             ************************************************/
            void print_key_generator();

            /*************************************************
             * Getter and Setter for Key Count
             ************************************************/
            int get_key_count() { return key_count; }
            void set_key_count(unsigned int kc) { key_count = kc; }

            /*************************************************
             * Getter and Setter for Key Size
             ************************************************/
            int get_key_size() { return key_size; }
            void set_key_size(unsigned int ks) { key_size = ks; }

            /*************************************************
             * Getter for all_key_list reference
             ************************************************/
            QList<unsigned char*>* get_ref_to_key_list() { return all_key_list; }

            /*************************************************
             * Getter for omega matrix reference
             ************************************************/
            int* get_ref_to_omega_matrix() { return omega_matrix; }

            /*************************************************
             * Getter for the size of the omega table
             ************************************************/
            int get_size_of_omega_table() { return n * key_count_per_machine; }

            /*************************************************
             * Getter for key count per machine
             ************************************************/
            int get_key_count_per_machine() { return key_count_per_machine; }

            /*************************************************
             * Getter for key size
             ************************************************/
            int get_size_of_each_key() { return key_size; }

            /*************************************************
             * Getter for n
             ************************************************/
            int get_n() {return n;}

            /*************************************************
             * Getter for t
             ************************************************/
            int get_t() {return t;}
    };

#endif