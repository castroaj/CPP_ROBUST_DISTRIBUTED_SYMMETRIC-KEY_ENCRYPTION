#!/bin/bash

# Script to compile and run the dise server
echo "Compiling DISE Server."
cd src
qmake && make

echo "Starting DISE Server."
cd ..
./src/DiseServer