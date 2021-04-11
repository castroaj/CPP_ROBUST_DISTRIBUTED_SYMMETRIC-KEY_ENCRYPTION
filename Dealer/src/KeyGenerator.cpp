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
                n = 5;
                t = 3;
                key_size = 32;
                key_count = factorial(n) / (factorial(t-1) * factorial(t)); // 10
                key_count_per_machine = factorial(n-1) / (factorial(t-1) * factorial(n - t)); // 6
                key_idx = 0;
                omega_col_idxs = (int*) calloc(n, sizeof(int));
                omega_matrix = (int*) calloc(n*key_count_per_machine, sizeof(int)); // n x num_seats 
                create_all_key_list();
                //create_omega_table();
                create_key_table();
                break;
            }
        case 1:
            {
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

int KeyGenerator::factorial(int x)
{
    if(x > 1)
        return x * factorial(x - 1);
    else
        return 1;
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

        for ( int j = 0; j < key_size; j++)
        {
            printf("%X ", bytes[j]);
        }
        cout << "\n";
    }
}

void KeyGenerator::create_all_key_list()
{
    #pragma omp parallel for
    for (int i = 0; i < key_count; i++)
    {
        unsigned char* cur_key = new unsigned char[key_size];
        RAND_bytes(cur_key, key_size);

        #pragma omp critical
        all_key_list->append(cur_key);
    }
}

void KeyGenerator::create_omega_table()
{
    generate_omega_matrix();
    print_omega_table();
}

void KeyGenerator::create_key_table()
{
    
}

void KeyGenerator::populate_omega_table(int servers[], int key_idx) {

    for (int i = 0; i < t; i++) {
        *(omega_matrix + servers[i]*key_count_per_machine + omega_col_idxs[servers[i]]) = key_idx + 1;
        *(omega_col_idxs + servers[i]) += 1;
    }

}

void KeyGenerator::find_combos(int server_combo[], int start_num, int combo_idx) {
    
    if (combo_idx == t) {
        populate_omega_table(server_combo, key_idx++);
        return;
    }

    for (int server_num = start_num; server_num<=n-1 && n-1-server_num+1 >= t-combo_idx; server_num++) {
        server_combo[combo_idx] = server_num;
        find_combos(server_combo, server_num+1, combo_idx+1);
    }
}

void KeyGenerator::generate_omega_matrix() {
    
    int* server_combo = (int*) malloc(t);
    find_combos(server_combo, 0, 0);
    free(server_combo);
}

void KeyGenerator::print_omega_table()
{
    for (int row = 0; row < n; row++) {
      printf("\n");
      for (int col = 0; col < key_count_per_machine; col++) {
         printf("%d ", *(omega_matrix + row*key_count_per_machine + col));
      }
    }
    printf("\n");
}