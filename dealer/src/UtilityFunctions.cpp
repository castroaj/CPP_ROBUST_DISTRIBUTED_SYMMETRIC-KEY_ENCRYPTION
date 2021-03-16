#include "../hdr/UtilityFunctions.h"

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