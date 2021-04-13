#include "../hdr/DealerDriver.h"
#include "../hdr/Environment.h"
#include "../hdr/KeyGenerator.h"
#include "../hdr/DealerClient.h"

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
                case hash("DEALER_IP_ADDRESSES"):
                    {
                        value = line.substr(delimPos + 1);

                        QString qvalue = QString::fromUtf8(value.c_str());
                        QStringList qstringlist = qvalue.split(QLatin1Char(','));
                        auto addresses = enviorment->get_ref_to_addresses();

                        for (int i = 0; i < qstringlist.size(); i++)
                        {
                            addresses->append(qstringlist.at(i));
                        }

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
                        break;
                    }
                // Set distribution mode for the dealing process
                case hash("DISTRIBUTION_MODE"):
                    {
                        value = line.substr(delimPos + 1);
                        enviorment->set_dist_mode(atoi(value.c_str()));
                        break;
                    }
                default:
                    {
                        value = line.substr(delimPos + 1);
                        break;
                    }
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
    // Setup Networking Server
    QCoreApplication app(argc, argv);
    
    QCommandLineParser parser;
    QCommandLineOption dbgOption(QStringList() << "d" << "debug", QCoreApplication::translate("main", "Debug Output [default: off]."));
    QCommandLineOption portOption(QStringList() << "p" << "port", QCoreApplication::translate("main", "Port for server [default: 1234]"), QCoreApplication::translate("main", "port"), QLatin1String("1234"));
    QCommandLineOption configOption(QStringList() << "c" << "config", "Fullpath and extension of input <file>", "file");

    parser.setApplicationDescription("Networking Setup");
    parser.addHelpOption();
    parser.addOption(dbgOption);
    parser.addOption(portOption);
    parser.addOption(configOption);
    parser.process(app);

    QString inputFileName = parser.value(configOption);
    bool debug = parser.isSet(dbgOption);
    //int port = parser.value(portOption).toInt();

    // Declare/Initalize key data structures for the application
    Environment* environment = new Environment();
    KeyGenerator* key_gen;

    // Depending of if a config file was provided to the application
    if (!inputFileName.isEmpty())
    {
        if (!setup_env_with_conf(inputFileName.toStdString(), environment))
            return EXIT_FAILURE;
    }
    else
    {
        if (!setup_env_with_conf("../config/Setup.conf", environment))
            return EXIT_FAILURE;
    }

    if (environment->get_dist_mode() == 0 || environment->get_dist_mode() == 1)
        key_gen = new KeyGenerator(environment->get_dist_mode());
    else
        return EXIT_FAILURE;


    if (debug)
    {
        environment->print_environment();
        //key_gen->print_key_generator();
    }

    QList<QString>* addresses = environment->get_ref_to_addresses();

    #pragma omp parallel for
    for (int i = 0; i < addresses->size(); i++)
    {
        DealerClient* client = new DealerClient(debug);

        QString curString = addresses->at(i);
        QStringList l = curString.split(QLatin1Char(':'));
        QString ip = l.at(0);
        QString port = l.at(1);

        if (debug)
            qDebug() << ip << " " << port.toInt();

        client->doConnect(key_gen, ip, port.toInt(), i);
    }

    std::cout << "Dealer Finished" << std::endl;
}
