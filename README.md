# CPP_ROBUST_DISTRIBUTED_SYMMETRIC-KEY_ENCRYPTION
Group: Alex Castro, Steve Chang, Garrett Christian, Rachel Litscher

---
## Overview
This is the code for our implementation of Professor Wang's robust distributed symmetric-key encryption scheme. 
Created for our parallel and distributed systems research project to find parallel improvements, analyse communication costs in the system, and to containerize the application.
Professor Wang's paper: [Robust distributed symmetric-key encryption](https://eprint.iacr.org/2020/1001)

---
## Dependencies
- g++
- build-essential
- libomp-dev
- libssl-dev
- qt5-default

---
## How to Compile and Run

### Server Local Version
- **All the servers should be run from different distributed machines**
- ./run.sh
- Or:
- cd into server folder
- qmake
- make
- cd ..
- ./src/server [OPTIONS]
- options:
 -c *config file* (a config file see example config in Dealer/config/Setup.conf)
 -d (debug)
 -p *port* (port to connect to)

### Server Local Docker Version
- sudo docker build . -t diseserver:1
- docker run --network=host diseserver:1

### Server Google Cloud Run Version (Currently Unimplemented)
- gcloud builds submit --tag gcr.io/distributed-cryptography/[image name]
- gcloud run deploy --image gcr.io/distributed-cryptography/[image name] --platform managed

### Dealer
- **all servers must be running to run the driver**
- cd into driver folder
- qmake
- make
- ./dealer [OPTIONS]
- options:
 -c *config file* (a config file see example config in Dealer/config/Setup.conf)
 -d (debug)
 -p *port* (port to connect to)

### Client (Currently Unimplemented)
- **all servers must be running to run the client**
- cd into client folder
- qmake
- make
- ./client

---
## Overview of Roles

### Driver
1. Generate the Key Matrix
2. Generate all the Keys
3. Send to each dise server: 
 The key matrix
 The other server's ip addresses
 The partial keys to that dise server

### Client
1. Randomly choose an honest initiator of the dise servers
2. Send to the randomly chosen honest initiator:
 Whether it is encryption or decryption
 The message to encrypt or decrypt

### Dise Server
1. Check if there are existing keys
2. If not wait for a dealer to send the keys
3. Wait indefinitely for a transaction from either:
 A client where an honest initiator process will be spawned
 A honest initiator where a participant server will be spawned

#### Honest Initiator
1. Randomly select participant servers
2. Split the message up into t parts
3. Send to a random combination of participant servers:
 Whether it is encryption or decryption
 What keys those servers will use
 The partial message
4. Encrypt or decrypt with all of the owned keys
5. Receive all partial results
6. Compare the partial results to determine if there was a compromised participating server
7. Compile the partial results into the final message
8. Send to the client:
 The resulting message
 If there was a compromised server
9. Conclude this transaction

#### Participant Server
1. Encrypt or decrypt using the specified keys
2. Send to the honest initiator:
 the partial results 
3. Conclude this transaction

---
## Communication patterns

### Driver to servers
![driver to servers](https://github.com/castroaj/CPP_ROBUST_DISTRIBUTED_SYMMETRIC-KEY_ENCRYPTION/blob/main/Diagrams/DealerServerFlow.pdf "Driver Flow.")

### Client to servers
![client to servers](https://github.com/castroaj/CPP_ROBUST_DISTRIBUTED_SYMMETRIC-KEY_ENCRYPTION/blob/main/Diagrams/ClientServerFlow.pdf "Client Transaction Flow.")
