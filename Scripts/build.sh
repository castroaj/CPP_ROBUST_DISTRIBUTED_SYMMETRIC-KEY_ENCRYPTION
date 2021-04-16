##!/bin/bash
cd ../Dealer/src
qmake && make
cd ../..

cd DISE_Server/src
qmake && make
cd ../..

cd Client/src
qmake && make
