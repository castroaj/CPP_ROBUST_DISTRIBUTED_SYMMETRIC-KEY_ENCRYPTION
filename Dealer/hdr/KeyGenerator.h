#ifndef KEY_GEN_H
    #define KEY_GEN_H
    #include "DealerDriver.h"

    class KeyGenerator
    {
        private:
            unsigned int key_count;
            unsigned int key_size;
            QList<unsigned char*>* all_key_list;
            QMap<QString, QList<int>>* omega_table;
            QMap<QString, QList<unsigned char*>>* key_table;

            void create_all_key_list();
            void create_omega_table();
            void create_key_table();

        public:
            KeyGenerator(unsigned int dist_mode, QList<QString>* addresses);
            ~KeyGenerator();

            void print_key_generator();

            unsigned int get_key_count() { return key_count; }
            void set_key_count(unsigned int kc) { key_count = kc; }

            unsigned int get_key_size() { return key_size; }
            void set_key_size(unsigned int ks) { key_size = ks; }

            QList<unsigned char*>* get_ref_to_key_list() { return all_key_list; }

            QMap<QString, QList<int>>* get_ref_to_omega_table() { return omega_table; }

            QMap<QString, QList<unsigned char*>>* get_ref_to_key_table() { return key_table; }
    };

#endif