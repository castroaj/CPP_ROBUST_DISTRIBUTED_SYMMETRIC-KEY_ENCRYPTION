# most of the examples use ubuntu as base and this is what we build it on
FROM ubuntu:20.04

# tell docker not to ask questions: 
# https://askubuntu.com/questions/909277/avoiding-user-interaction-with-tzdata-when-installing-certbot-in-a-docker-contai
ARG DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && \
    apt-get install -y \
        g++ \
        build-essential \
        libomp-dev \
        libssl-dev \
        qt5-default 

# copy the files
COPY . /usr/src/Dealer

# set the work directory
WORKDIR /usr/src/Dealer

# compile
RUN cd src && qmake && make

# run the server
CMD ./src/Dealer -c ./config/3_5.conf
