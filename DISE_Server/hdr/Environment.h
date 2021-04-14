#ifndef ENVIRONMENT_H
    #define ENVIRONMENT_H
    #include "DISEServerDriver.h"
    
    class Environment
    {
        private:
            unsigned int threadCount;
            unsigned int distMode;
            int machineNum;
            int keysPerMachine;
            int sizeOfEachKey;
            int N;
            int T;

            QList<QPair<QString, int>*>* addresses;
            QMap<int, int*>* omegaTable;
            QMap<int, unsigned char*>* keyList;

        public:
            Environment();
            ~Environment();

            void print_environment();

            unsigned int get_thread_count() { return threadCount; }
            void set_thread_count(unsigned int tc) { threadCount = tc; }

            unsigned int get_dist_mode() { return distMode; }
            void set_dist_mode(unsigned int dm) { distMode = dm; }

            int get_machine_num() { return machineNum; }
            void set_machine_num(int mn) { machineNum = mn; }

            int get_keys_per_machine() { return keysPerMachine; }
            void set_keys_per_machine(int kpm) { keysPerMachine = kpm; }

            int get_size_of_each_key() { return sizeOfEachKey; }
            void set_size_of_each_key(int size) { sizeOfEachKey = size; }

            int get_N() { return N; }
            void set_N(int n) { N = n; }

            int get_T() { return T; }
            void set_T(int t) { T = t; }

            QList<QPair<QString, int>*>* get_ref_to_addresses() { return addresses; }
            void set_ref_to_addresses(QList<QPair<QString, int>*>* a) { addresses = a; }

            QMap<int, int*>* get_ref_to_omega_table() { return omegaTable; }
            void set_ref_to_omega_table(QMap<int, int*>* ot) { omegaTable = ot; }

            QMap<int, unsigned char*>* get_ref_to_key_list() { return keyList; }
            void set_ref_to_key_list(QMap<int, unsigned char*>* kl) { keyList = kl; }
    };

#endif