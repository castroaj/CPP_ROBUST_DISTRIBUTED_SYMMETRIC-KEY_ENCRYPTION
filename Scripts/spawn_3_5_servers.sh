#!/bin/bash

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