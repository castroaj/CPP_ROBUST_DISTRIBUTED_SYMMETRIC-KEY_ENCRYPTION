#include "../hdr/KeyGenerator.h"

KeyGenerator::KeyGenerator(unsigned int dist_mode, QList<QString>* addresses)
{
    all_key_list = new QList<unsigned char*>();
    omega_table = new QMap<QString, QList<int>>();
    key_table = new QMap<QString, QList<unsigned char*>>();

    switch (dist_mode)
    {
        case 0:
            {
                key_count = 10;
                key_size = 32;
                create_all_key_list();
                create_omega_table();
                create_key_table();
                break;
            }
        case 1:
            {
                // UNSUPPPORTED RN
                break;
            }
        default:
            break;
    }
}

KeyGenerator::~KeyGenerator()
{
    // Free memory for all-key list
    for (int i = 0; i < all_key_list->size();i++)
    {
        free(all_key_list->at(i));
    }
    delete all_key_list;

    // Free memory for omega table
    omega_table->clear();
    delete omega_table;

    // Free memory for key table
    key_table->clear();
    delete key_table;
}

void KeyGenerator::print_key_generator()
{
    using namespace std;

    cout << "Key Count: " << key_count << endl;
    cout << "Key Size: " << key_size << endl;

    cout << "All Keys: " << endl;

    for (int i = 0; i < all_key_list->size(); i++)
    {
        cout << "\t" << i << ": ";

        auto bytes = all_key_list->at(i);

        for (unsigned int j = 0; j < key_size; j++)
        {
            printf("%X ", bytes[j]);
        }
        cout << "\n";
    }
}

void KeyGenerator::create_all_key_list()
{
    #pragma omp parallel for
    for (unsigned int i = 0; i < key_count; i++)
    {
        unsigned char* cur_key = new unsigned char[key_size];
        RAND_bytes(cur_key, key_size);

        #pragma omp critical
        all_key_list->append(cur_key);
    }
}

void KeyGenerator::create_omega_table()
{
    
}

void KeyGenerator::create_key_table()
{

}