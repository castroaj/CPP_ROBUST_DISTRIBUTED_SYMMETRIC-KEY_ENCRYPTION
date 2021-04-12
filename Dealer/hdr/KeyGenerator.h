#ifndef KEY_GEN_H
    #define KEY_GEN_H
    #include "DealerDriver.h"

    class KeyGenerator
    {
        private:
            int key_count;
            int key_count_per_machine;
            int key_size;
            int n;
            int t;
            int key_idx;
            int* omega_col_idxs;
            int* omega_matrix; 
            QList<unsigned char*>* all_key_list;
            QMap<QString, QList<int>>* omega_table;
            QMap<QString, QList<unsigned char*>>* key_table;

            void create_all_key_list();
            void create_omega_table();
            void create_key_table();
            void populate_omega_table(int servers[], int key_idx);
            void find_combos(int server_combo[], int start_num, int combo_idx);
            void generate_omega_matrix();
            void print_omega_table();
            unsigned nChoosek(unsigned n, unsigned k);


        public:
            KeyGenerator(unsigned int dist_mode);
            ~KeyGenerator();
            void print_key_generator();
            int get_key_count() { return key_count; }
            void set_key_count(unsigned int kc) { key_count = kc; }
            int get_key_size() { return key_size; }
            void set_key_size(unsigned int ks) { key_size = ks; }
            QList<unsigned char*>* get_ref_to_key_list() { return all_key_list; }
            QMap<QString, QList<int>>* get_ref_to_omega_table() { return omega_table; }
            QMap<QString, QList<unsigned char*>>* get_ref_to_key_table() { return key_table; }
            int* get_ref_to_omega_matrix() { return omega_matrix; }
            int get_size_of_omega_table() { return n * key_count_per_machine; }
            int get_key_count_per_machine() { return key_count_per_machine; }
            int get_size_of_each_key() { return key_size; }
    };

#endif