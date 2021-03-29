#ifndef ENVIRONMENT_H
    #define ENVIRONMENT_H
    #include "DISEServerDriver.h"
    
    class Environment
    {
        private:
            unsigned int thread_count;
            unsigned int dist_mode;
            unsigned int port;
            QList<QString>* ip_addresses;
        
        public:
            Environment();
            ~Environment();

            void print_environment();

            unsigned int get_thread_count() { return thread_count; }
            void set_thread_count(unsigned int tc) { thread_count = tc; }

            unsigned int get_dist_mode() { return dist_mode; }
            void set_dist_mode(unsigned int dm) { dist_mode = dm; }

            unsigned int get_port() { return port; }
            void set_port(unsigned int p) { port = p; }

            QList<QString>* get_ref_to_addresses() { return ip_addresses; }
    };

#endif