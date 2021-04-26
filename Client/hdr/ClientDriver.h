/*******************************************
 * List of C++ libraries that will be used
 * within the Client Application 
 *******************************************/

#ifndef CLIENT_DRIVER_H
    #define CLIENT_DRIVER_H
    #include <iostream>
    #include <vector>
    #include <unordered_map>
    #include <fstream>
    #include <algorithm>
    #include <cstring>
    #include <stdio.h>
    #include <inttypes.h>
    #include <openssl/rand.h>
    #include "Qt.h"
    #include <cstdio>
    #include <ctime>
    #include <chrono>
    #define ENCRYPTION 0
    #define DECRYPTION 1
#endif

#ifdef _OPENMP
    #include <omp.h>
#endif