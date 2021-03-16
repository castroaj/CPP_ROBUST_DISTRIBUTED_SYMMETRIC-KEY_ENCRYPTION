#include "../hdr/Environment.h"

Environment::Environment()
{
    ip_addresses = new std::vector<std::string>();
    thread_count = 1;
    dist_mode = 0;
}

Environment::~Environment()
{
    ip_addresses->clear();
    delete &ip_addresses;
}

void Environment::print_environment()
{
    using namespace std;

    cout << "Thread Count: " << thread_count << endl;
    cout << "Distribution mode: " << dist_mode << endl;
    
    for (unsigned int i = 0; i < ip_addresses->size(); i++)
        cout << "Address " << (i+1) << ": " << ip_addresses->at(i) << endl;
}