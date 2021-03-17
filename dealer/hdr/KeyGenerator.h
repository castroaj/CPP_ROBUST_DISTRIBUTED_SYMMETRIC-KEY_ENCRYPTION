#include "DealerDriver.h"

#ifndef KEY_GEN_H
#define KEY_GEN_H

class KeyGenerator
{
    private:
        unsigned int key_count;
        unsigned int key_size;
        std::vector<unsigned char*>* all_key_list;
        std::unordered_map<std::string, std::vector<int>>* omega_table;
        std::unordered_map<std::string, std::vector<unsigned char*>>* key_table;

        void create_all_key_list();
        void create_omega_table();
        void create_key_table();

    public:
        KeyGenerator(unsigned int dist_mode, std::vector<std::string>* addresses);
        ~KeyGenerator();

        void print_key_generator();

        unsigned int get_key_count() { return key_count; }
        void set_key_count(unsigned int kc) { key_count = kc; }

        unsigned int get_key_size() { return key_size; }
        void set_key_size(unsigned int ks) { key_size = ks; }

        std::vector<unsigned char*>* get_ref_to_key_list() { return all_key_list; }

        std::unordered_map<std::string, std::vector<int>>* get_ref_to_omega_table() { return omega_table; }

        std::unordered_map<std::string, std::vector<unsigned char*>>* get_ref_to_key_table() { return key_table; }
};

#endif