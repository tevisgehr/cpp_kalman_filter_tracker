#include <iostream>
#include <thread>
#include <mutex>
#include <cmath>

#include "TrackedObject.h"

int TrackedObject::_idCount = 0;

template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] {return !_queue.empty();});
    T msg = std::move(_queue.back());
    _queue.pop_back();

    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{   
    std::lock_guard<std::mutex> uLock(_mutex);
    _queue.push_back(std::move(msg));
    _cond.notify_one();
}

void TrackedObject::sendDetection(std::shared_ptr<Detection> &det){
    _detectionQueue.send(std::move(det));
}

TrackedObject::TrackedObject(std::shared_ptr<Detection> newDet) : _id(_idCount) {
    ++_idCount;
    cout_mtx_.lock();
    std::cout<<_id<<"Creating new tracked object from detection at: ("<<newDet->x_mid<<","<<newDet->y_mid<<")"<< std::endl;
    cout_mtx_.unlock();

    _X.resize(2,2);
    _X(0,0) = newDet->x_mid;
    _X(1,0) = 0;
    _X(0,1) = newDet->y_mid;
    _X(1,1) = 0;
}

void TrackedObject::run(){
    while (true){
        // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        cout_mtx_.lock();
        std::cout<<"Receiving message..." << std::endl;
        cout_mtx_.unlock();

        auto newDetection = _detectionQueue.receive();
        cout_mtx_.lock();
        std::cout<<_id<<" received Message!!!!! det at: ("<<newDetection->x_mid<<","<<newDetection->y_mid<<")"<< std::endl;
        cout_mtx_.unlock();

        cout_mtx_.lock();
        std::cout<<"Object ID:"<<_id<<" Main loop." << std::endl;
        cout_mtx_.unlock();

    }
}

void TrackedObject::timeUpdate(){}
void TrackedObject::measurementUpdate(){}

float TrackedObject::measureDistance(std::shared_ptr<Detection> det){
    return std::sqrt(std::pow((det->x_mid-_X(0,0)),2)+std::pow((det->y_mid-_X(0,1)),2));
    }