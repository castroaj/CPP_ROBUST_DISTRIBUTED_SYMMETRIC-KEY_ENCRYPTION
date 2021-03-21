#ifndef ENVIRONMENT_H
    #define ENVIRONMENT_H
    #include "DealerDriver.h"
    
    class Environment
    {
        private:
            unsigned int thread_count;
            unsigned int dist_mode;
            QList<QString>* ip_addresses;
        
        public:
            Environment();
            ~Environment();

            void print_environment();

            unsigned int get_thread_count() { return thread_count; }
            void set_thread_count(unsigned int tc) { thread_count = tc; }

            unsigned int get_dist_mode() { return dist_mode; }
            void set_dist_mode(unsigned int dm) { dist_mode = dm; }

            QList<QString>* get_ref_to_addresses() { return ip_addresses; }
    };

#endif