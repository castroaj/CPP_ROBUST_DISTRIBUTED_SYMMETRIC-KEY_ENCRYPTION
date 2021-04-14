#ifndef ENVIRONMENT_H
    #define ENVIRONMENT_H
    #include "ClientDriver.h"
    
    class Environment
    {
        private:
            unsigned int thread_count;
            unsigned int enc_mode;
            QList<QString>* ip_addresses;
        
        public:
            Environment();
            ~Environment();

            void print_environment();

            unsigned int get_thread_count() { return thread_count; }
            void set_thread_count(unsigned int tc) { thread_count = tc; }

            unsigned int get_enc_mode() { return enc_mode; }
            void set_enc_mode(unsigned int enc) { enc_mode = enc; }

            QList<QString>* get_ref_to_addresses() { return ip_addresses; }
    };

#endif