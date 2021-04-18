#!/bin/bash

echo "RUNNING DISE EXCHANGE WITH (N=24, T=16)"
./clean_output.sh

mkdir output_16_24
mkdir output_16_24/DiseServer
mkdir output_16_24/Dealer
mkdir output_16_24/Client

cd ../DISE_Server/src
ports=("1234" "1235" "1236" "1237" "1238" "1239" "1240" "1241" "1242" "1243" "1244"
       "1245" "1246" "1247" "1248" "1249" "1250" "1251" "1252" "1253" "1254" "1255"
       "1256" "1257")

echo "LAUNCHING DISE SERVERS"
for i in "${ports[@]}"; do
    ./DiseServer -d -p $i > ../../Scripts/output_16_24/DiseServer/DiseServer_$i &
done

cd ../../Dealer/src
echo "DEALING"
./Dealer -d -c ../config/n24t16Local.conf > ../../Scripts/output_16_24/Dealer/Dealer

echo "SLEEPING TO ALLOW FOR COMMUNICATION BEFORE CLIENT"
sleep 10

echo "CLIENT REQUEST"
cd ../../Client/src
./Client -d > ../../Scripts/output_16_24/Client/Client

echo "KILLING DISE SERVERS"
killall DiseServer
echo "FINISHED"
cd ../../Scripts/output_16_24/Dealer

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