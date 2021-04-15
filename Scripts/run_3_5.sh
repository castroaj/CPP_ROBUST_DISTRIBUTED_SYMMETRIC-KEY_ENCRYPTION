#!/bin/bash

echo "RUNNING DISE EXCHANGE WITH (N=5, T=3)"
./clean_output.sh

mkdir output_3_5
mkdir output_3_5/DiseServer
mkdir output_3_5/Dealer
mkdir output_3_5/Client

cd ../DISE_Server/src
ports=("1234" "1235" "1236" "1237" "1238")

echo "LAUNCHING DISE SERVERS"
for i in "${ports[@]}"; do
    ./DiseServer -d -p $i > ../../Scripts/output_3_5/DiseServer/DiseServer_$i &
done

cd ../../Dealer/src
echo "DEALING"
./Dealer -d -c ../config/n5t3Local.conf > ../../Scripts/output_3_5/Dealer/Dealer 

echo "SLEEPING TO ALLOW FOR COMMUNICATION BEFORE CLIENT"

echo "CLIENT REQUEST"
cd ../../Client/src
./Client -d > ../../Scripts/output_3_5/Client/Client

echo "SLEEPING TO ALLOW FOR COMMUNICATION BEFORE KILLING SERVERS"

echo "KILLING DISE SERVERS"
killall DiseServer

cd ../../Scripts/output_3_5/Dealer

echo
echo
echo
echo "DEALER OUTPUT"
echo "============="
cat Dealer

echo 
echo
echo
sleep 2

cd ../Client
echo "Client OUTPUT"
echo "============="
cat Client

echo
echo
echo
sleep 2
cd ../DiseServer

for i in "${ports[@]}"; do
    echo "SERVER LOCALHOST:$i"
    cat DiseServer_$i
    echo
    echo
    echo
    sleep 1
done
echo
echo "FINISHED"
