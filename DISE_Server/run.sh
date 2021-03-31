#!/bin/bash

# Script to compile and run the dise server
echo "Starting DISE Server."
cd src
qmake && make

cd ..
./src/DiseServer