#include "../hdr/DealerDriver.h"

void string_split(std::string input, std::string delim, std::vector<std::string>* arr)
{
    size_t pos = 0;
    std::string token;

    while((pos = input.find(delim)) != std::string::npos)
    {
        token = input.substr(0, pos);
        arr->push_back(token);
        input.erase(0, pos + 1);
    }

    if (input != "")
        arr->push_back(input);
}

constexpr unsigned int hash(const char *s, int off = 0) {                        
    return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];                           
} 

bool setup_env_with_conf(std::string cFilePath)
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
                case hash("IP_ADDRESSES"):
                    {
                        value = line.substr(delimPos + 1);
                    
                        // Parses the line into a vector of addresses, this will be put in object
                        std::vector<std::string> addresses;
                        string_split(value, ",", &addresses);

                        std::cout << addresses.size() << std::endl;
                        break;
                    }
                default:
                    value = line.substr(delimPos + 1);
                    break;
            }

            std::cout << name << " " << value << std::endl;
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
    if (argc > 1)
    {
        if (!setup_env_with_conf(argv[argc-1]))
            return EXIT_FAILURE;
    }
    else
    {
        if (!setup_env_with_conf("dealer/config/Setup.conf"))
            return EXIT_FAILURE;
    }

}
