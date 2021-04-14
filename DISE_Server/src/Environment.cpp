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
    cout << "\tDistribution mode: " << distMode << endl;
    cout << "\tMachineNum: " << machineNum << endl;
    cout << "\tKeysPerMachine: " << keysPerMachine << endl;
    cout << "\tSizeOfEachKey: " << sizeOfEachKey << endl;
    cout << "\tN: " << N << endl;
    cout << "\tT: " << T << endl;

    cout << "\tADDRESSES:" << endl;
    
    for (int i =0 ; i < addresses->size(); i++)
    {
        auto t = addresses->at(i);

        qDebug() << "\t" << i << ": " << t->first << ":" << t->second;
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
        QMapIterator<int, int*> iter2(*omegaTable);

        while (iter2.hasNext())
        {
            iter2.next();
            std::cout << "\t" << iter2.key() << ": ";

            int* v = iter2.value();

            for (int i = 0; i < keysPerMachine; i++)
                printf("%d ", *(v + i));
            
            std::cout << "\n";
        }
    }
    else
    {
        cout << "\t\tMATRIX RECIEVED IS TOO BIG TO PRINT" << endl;
    }
}