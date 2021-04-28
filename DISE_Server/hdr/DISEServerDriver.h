/*******************************************
 * List of C++ libraries that will be used
 * within the Client Application 
 *******************************************/

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
    #include <openssl/sha.h>
    #include <openssl/rand_drbg.h>
    #include <openssl/crypto.h>
    #define MAX_CIPHER_LEN 1024
    #define ENCRYPTION 0
    #define DECRYPTION 1
    #define A_BYTE_SIZE 32
#endif

#ifdef _OPENMP
    #include <omp.h>
#endif