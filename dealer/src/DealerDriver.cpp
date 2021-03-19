#include "../hdr/DealerDriver.h"
#include "../hdr/Environment.h"
#include "../hdr/KeyGenerator.h"
#include "../hdr/UtilityFunctions.h"

constexpr unsigned int hash(const char *s, int off = 0) 
{                        
    return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];                           
} 

bool setup_env_with_conf(std::string cFilePath, Environment* enviorment)
{
    // Input file stream
    std::ifstream cFile(cFilePath);

    // Validate the file could be opened
    if (cFile.is_open())
    {
        // Line to hold incoming data
        std::string line;

        // String to hold enviorment data
        std::string value;

        // Parse while the file stream has more lines
        while (getline(cFile, line))
        {
            // Get rid of any surronding whitespace
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

            // Parse out comments
            if (line[0] == '#' || line.empty())
                continue;
            
            // Find the delimiting position
            auto delimPos = line.find("=");

            // Get the name of the enviorment variable
            auto name = line.substr(0, delimPos);

            // Switch based on enviorment variable name
            switch (hash(name.c_str()))
            {
                // Setup vector of ip addresses that will be the locations of the machines
                case hash("IP_ADDRESSES"):
                    {
                        value = line.substr(delimPos + 1);
                    
                        auto addresses = enviorment->get_ref_to_addresses();
                        string_split(value, ",", addresses);

                        break;
                    }
                // Set the thread count for all openmp_operations
                case hash("THREAD_COUNT"):
                    {
                        value = line.substr(delimPos + 1);
                        enviorment->set_thread_count(atoi(value.c_str()));

                        #ifdef _OPENMP
                            omp_set_num_threads(atoi(value.c_str()));
                        #endif
                    }
                // Set distribution mode for the dealing process
                case hash("DISTRIBUTION_MODE"):
                    {
                        value = line.substr(delimPos + 1);
                        enviorment->set_dist_mode(atoi(value.c_str()));
                    }
                default:
                    value = line.substr(delimPos + 1);
                    break;
            }
        }
        return true;
    }
    else
    {
        std::cerr << "Couldn't find a config file for reading" << std::endl;
        return false;
    }
}

int main(int argc, char* argv[])
{   
    // Declare/Initalize key data structures for the application
    Environment* environment = new Environment();
    KeyGenerator* key_gen;

    // Depending of if a config file was provided to the application
    if (argc > 1)
    {
        if (!setup_env_with_conf(argv[argc-1], environment))
            return EXIT_FAILURE;
    }
    else
    {
        if (!setup_env_with_conf("dealer/config/Setup.conf", environment))
            return EXIT_FAILURE;
    }

    // NEEDS TO BE REMOVED IN FUTURE
    if (environment->get_dist_mode() == 1)
    {
        std::cout << "Currently Unsupported" << std::endl;
        return EXIT_FAILURE;
    }

    if (environment->get_dist_mode() == 0 || environment->get_dist_mode() == 1)
        key_gen = new KeyGenerator(environment->get_dist_mode(), environment->get_ref_to_addresses());
    else
        return EXIT_FAILURE;

    key_gen->print_key_generator();
}
