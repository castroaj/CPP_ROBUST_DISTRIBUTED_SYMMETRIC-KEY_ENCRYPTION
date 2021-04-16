#ifndef DISE_SERVER_DRIVER_H
    #define DISE__SERVER_DRIVER_H
    #include <iostream>
    #include <vector>
    #include <unordered_map>
    #include <fstream>
    #include <algorithm>
    #include <cstring>
    #include <stdio.h>
    #include <inttypes.h>
    #include <openssl/rand.h>
    #include <tuple>
    #include "Qt.h"
    #include <string>
    #include <thread> 
    #include <mutex> 
    #include <openssl/evp.h>
    #define MAX_CIPHER_LEN 1024
#endif

#ifdef _OPENMP
    #include <omp.h>
#endif