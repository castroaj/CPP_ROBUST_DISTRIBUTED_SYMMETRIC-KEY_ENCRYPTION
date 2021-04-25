#!/bin/bash

# build
echo "BUILD"
./build

# start servers
echo "BUILD"
gnome-terminal -x bash -c "../DISE_Server/src/DiseServer -d -p 1234; exec bash"
gnome-terminal -x bash -c "../DISE_Server/src/DiseServer -d -p 1235; exec bash"
gnome-terminal -x bash -c "../DISE_Server/src/DiseServer -d -p 1236; exec bash"
gnome-terminal -x bash -c "../DISE_Server/src/DiseServer -d -p 1237; exec bash"
gnome-terminal -x bash -c "../DISE_Server/src/DiseServer -d -p 1238; exec bash"

# sleep
echo "SLEEP BEFORE DEALER"
sleep(5)

gnome-terminal -x bash -c "../Dealer -d -p 1235; exec bash"

