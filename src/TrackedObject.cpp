#include <iostream>
#include <thread>
#include <mutex>
#include <cmath>

#include "TrackedObject.h"

int TrackedObject::_idCount = 0;

TrackedObject::TrackedObject(std::shared_ptr<Detection> newDet) : _id(_idCount) {
    ++_idCount;
    cout_mtx.lock();
    std::cout<<_id<<"Creating new tracked object from detection at: ("<<newDet->x_mid<<","<<newDet->y_mid<<")"<< std::endl;
    cout_mtx.unlock();

    _X.resize(2,2);
    _X(0,0) = newDet->x_mid;
    _X(1,0) = 0;
    _X(0,1) = newDet->y_mid;
    _X(1,1) = 0;
}

void TrackedObject::run(){
    while (true){
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        cout_mtx.lock();
        std::cout<<"Object ID:"<<_id<<" Main loop." << std::endl;
        cout_mtx.unlock();

    }
}

void TrackedObject::timeUpdate(){}
void TrackedObject::measurementUpdate(){}

float TrackedObject::measureDistance(std::shared_ptr<Detection> det){
    return std::sqrt(std::pow((det->x_mid-_X(0,0)),2)+std::pow((det->y_mid-_X(0,1)),2));
    }