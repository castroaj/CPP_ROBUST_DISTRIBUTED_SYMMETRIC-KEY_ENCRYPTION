##!/bin/bash
cd ../Dealer/src
make clean
rm Dealer
cd ../..

cd DISE_Server/src
make clean
rm DiseServer
cd ../..

cd Client/src
make clean
rm Client
