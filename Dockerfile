FROM ubuntu

RUN apt-get update && \ 
    apt install build-essential && \
    apt-get install software-properties-common && \
    add-apt-repository ppa:george-edison55/cmake-3.x && \
    apt-get install cmake


... in progress ...