#!/bin/bash

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