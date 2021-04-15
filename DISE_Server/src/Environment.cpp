#include "../hdr/Environment.h"

Environment::Environment()
{
}

Environment::~Environment()
{
}

void Environment::print_environment()
{
    using namespace std;

    cout << "ENVIRONMENT:" << endl;
    cout << "\tThread Count: " << threadCount << endl;
    cout << "\tMachineNum: " << machineNum << endl;
    cout << "\tTotalKeys: " << totalKeyNum << endl;
    cout << "\tKeysPerMachine: " << keysPerMachine << endl;
    cout << "\tSizeOfEachKey: " << sizeOfEachKey << endl;
    cout << "\tN: " << N << endl;
    cout << "\tT: " << T << endl;

    cout << "\tADDRESSES:" << endl;
    
    for (int i =0 ; i < addresses->size(); i++)
    {
        auto t = addresses->at(i);

        QTextStream(stdout) << "\t" << i << ": " << t->first << ":" << t->second << "\n";
    }

    cout << "\tKEY LIST:" << endl;
    if (N != 24)
    {
        QMapIterator<int, unsigned char*> iter(*keyList);
        while (iter.hasNext())
        {
            iter.next();
            cout << "\t" << iter.key() << ": ";

            unsigned char* v = iter.value();

            for (int i = 0; i < sizeOfEachKey; i++)
                printf("%x ", *(v + i));
            
            cout << "\n";
        }
    }
    else
    {
        cout << "\t\tKEY LIST RECIEVED IS TOO BIG TO PRINT" << endl;
    }

    cout << "\tOMEGA MATRIX" << endl;
    if (N != 24)
    {
        // iterate through n machines held keys
        QMap<int, QSet<int>*>::iterator rowIter;
        for (rowIter = omegaTable->begin(); rowIter != omegaTable->end(); ++rowIter)
        {
            std::cout << "\t" << rowIter.key() << ": ";

            // print key values in omega row
            QSet<int>* omegaRow = rowIter.value();
            QSet<int>::iterator keyIter;

            for (keyIter = omegaRow->begin(); keyIter != omegaRow->end(); ++keyIter)
                std::cout << *keyIter << " ";
            
            std::cout << "\n";
        }
    }
    else
    {
        cout << "\t\tMATRIX RECIEVED IS TOO BIG TO PRINT" << endl;
    }
}

bool Environment::server_owns_key(int server, int keyId)
{
    return omegaTable->contains(server) && omegaTable->value(server)->contains(keyId);
}