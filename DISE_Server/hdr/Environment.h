#ifndef ENVIRONMENT_H
    #define ENVIRONMENT_H
    #include "DISEServerDriver.h"
    
    class Environment
    {
        private:
            unsigned int threadCount;
            int machineNum;
            int keysPerMachine;
            int sizeOfEachKey;
            int totalKeyNum;
            int N;
            int T;

            QList<QPair<QString, int>*>* addresses;
            QMap<int, QSet<int>*>* omegaTable;
            QMap<int, unsigned char*>* keyList;

        public:
            Environment();
            ~Environment();

            void print_environment();

            unsigned int get_thread_count() { return threadCount; }
            void set_thread_count(unsigned int tc) { threadCount = tc; }

            int get_machine_num() { return machineNum; }
            void set_machine_num(int mn) { machineNum = mn; }

            int get_total_key_num() { return totalKeyNum; }
            void set_total_key_num(int tkn) { totalKeyNum = tkn; }

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

            QMap<int, QSet<int>*>* get_ref_to_omega_table() { return omegaTable; }
            void set_ref_to_omega_table(QMap<int, QSet<int>*>* ot) { omegaTable = ot; }

            QMap<int, unsigned char*>* get_ref_to_key_list() { return keyList; }
            void set_ref_to_key_list(QMap<int, unsigned char*>* kl) { keyList = kl; }

            bool server_owns_key(int server, int keyId);
    };

#endif