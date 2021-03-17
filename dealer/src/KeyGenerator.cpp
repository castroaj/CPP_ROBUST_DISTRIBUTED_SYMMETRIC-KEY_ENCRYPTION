#include "../hdr/KeyGenerator.h"

KeyGenerator::KeyGenerator(unsigned int dist_mode, std::vector<std::string>* addresses)
{
    all_key_list = new std::vector<unsigned char*>();
    omega_table = new std::unordered_map<std::string, std::vector<int>>();
    key_table = new std::unordered_map<std::string, std::vector<unsigned char*>>();

    switch (dist_mode)
    {
        case 0:
            {
                key_count = 1307504;
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

}

void KeyGenerator::print_key_generator()
{
    using namespace std;

    cout << "Key Count: " << key_count << endl;
    cout << "Key Size: " << key_size << endl;

    cout << "All Keys: " << endl;

    for (unsigned int i = 0; i < all_key_list->size(); i++)
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
        all_key_list->push_back(cur_key);
    }
    std::cout << "DONE" << std::endl;
}

void KeyGenerator::create_omega_table()
{

}

void KeyGenerator::create_key_table()
{

}