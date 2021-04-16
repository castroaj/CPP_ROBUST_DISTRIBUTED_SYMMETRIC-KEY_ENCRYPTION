#include "../hdr/Environment.h"

Environment::Environment()
{
    ip_addresses = new QList<QString>();
    thread_count = 1;
    enc_mode = 0;
}

Environment::~Environment()
{
    ip_addresses->clear();
    delete ip_addresses;
}

void Environment::print_environment()
{
    using namespace std;

    cout << "Thread Count: " << thread_count << endl;
    cout << "Encryption mode: " << enc_mode << endl;
    
    for (int i = 0; i < ip_addresses->size(); i++)
    {
        QString addr = ip_addresses->at(i).toLatin1();
        cout << "Address " << (i+1) << ": " << addr.toLocal8Bit().data() << endl;
    }
}