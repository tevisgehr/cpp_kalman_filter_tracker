#include <iostream>
#include <thread>
#include <mutex>

#include "TrackedObject.h"

int TrackedObject::_idCount = 0;

TrackedObject::TrackedObject(std::shared_ptr<Detection> newDet) : _id(_idCount) {
    ++_idCount;

    cout_mtx.lock();
    std::cout<<_id<<"Creating new tracked object from detection at: ("<<newDet->x_mid<<","<<newDet->y_mid<<")"<< std::endl;
    cout_mtx.unlock();
}

void TrackedObject::run(){
    while (true){
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        cout_mtx.lock();
        std::cout<<"Object ID:"<<_id<<" Main loop." << std::endl;
        cout_mtx.unlock();

    }
}