#include <iostream>
#include <thread>
#include <mutex>
#include <cmath>

#include "TrackedObject.h"

int TrackedObject::_idCount = 0;

//Initialize state transition matrix
Eigen::Matrix<float, 6, 6> TrackedObject::_A = [] {
    Eigen::Matrix<float, 6, 6> tmp;
    tmp <<  1,0,1,0,0,0,
            0,1,0,1,0,0,
            0,0,1,0,1,0,
            0,0,0,1,0,1,
            0,0,0,0,0,0,
            0,0,0,0,0,0;
    return tmp;
}();

//Initialize measurement matrix
Eigen::Matrix<float, 2, 6> TrackedObject::_H = [] {
    Eigen::Matrix<float, 2, 6> tmp;
    tmp <<  1,0,0,0,0,0,
            0,1,0,0,0,0;
    return tmp;
}();



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

void TrackedObject::timeUpdate(){
    cout_mtx_.lock();
    std::cout<<"Object ID:"<<_id<<" timeUpdate()" << std::endl;
    cout_mtx_.unlock();

    // Predict state transition
    _X = _A*_X;

    //Update state covariance 
    _P = _A*(_P*_A.transpose()) + _Q;

}

void TrackedObject::measurementUpdate(){

}

void TrackedObject::sendDetection(std::shared_ptr<Detection> det){
    _detectionQueue.send(std::move(det));
}

TrackedObject::TrackedObject(std::shared_ptr<Detection> newDet) : _id(_idCount) {
    ++_idCount;
    cout_mtx_.lock();
    std::cout<<_id<<"Creating new tracked object from detection at: ("<<newDet->x_mid<<","<<newDet->y_mid<<")"<< std::endl;
    cout_mtx_.unlock();

    // Initialize state vector with inital position
    _X <<  newDet->x_mid,newDet->y_mid,0,0,0,0;

    // Initialize Error Covariance matrix
    _P << _initalErrorCovariance,0,0,0,0,0,
          0,_initalErrorCovariance,0,0,0,0,
          0,0,_initalErrorCovariance,0,0,0,
          0,0,0,_initalErrorCovariance,0,0,
          0,0,0,0,_initalErrorCovariance,0,
          0,0,0,0,0,_initalErrorCovariance;

    // Initialize Measurement Covariance matrix
    _R << _measurmantVariance,0,
          0,_measurmantVariance;

    // Initialize Process Covariance matrix
    _Q << _processVariance,0,0,0,0,0,
          0,_processVariance,0,0,0,0,
          0,0,_processVariance,0,0,0,
          0,0,0,_processVariance,0,0,
          0,0,0,0,_processVariance,0,
          0,0,0,0,0,_processVariance;
}

void TrackedObject::run(){
    while (_state != terminated){    
        cout_mtx_.lock();
        std::cout<<"Object ID:"<<_id<<" Main loop." << std::endl;
        cout_mtx_.unlock();
        
        // Run predict step of Kalman filter
        if (_state != init){
            timeUpdate();
        }

        cout_mtx_.lock();
        std::cout<<"Object ID:"<<_id<<"Receiving message..." << std::endl;
        cout_mtx_.unlock();

        auto newDetection = _detectionQueue.receive();

        if (newDetection == nullptr){
            // If there is no new detection associated while the track is still in init phase, terminate it
            if (_state == init){
                _state = terminated;
            }
            else{
                _state = coast;
                ++_coastedFrames;
            }
            
            cout_mtx_.lock();
            std::cout<<"Track #"<<_id<<" received Message!!!!! nullptr... coasting. _coastedFrames: "<< _coastedFrames << std::endl;
            cout_mtx_.unlock();
        }
        else {
            // if this is the first associated detection when the tract is still in the init phase, 
            // initalize the velocity eimste
            if (_state == init){
                float vxEstimate = newDetection->x_mid - _X(0);
                float vyEstimate = newDetection->y_mid - _X(1);

                _X(2) = vxEstimate;
                _X(3) = vyEstimate;

                // Run first time update to catch up
                timeUpdate();
            }

            _state = active;
            _coastedFrames = 0;

            cout_mtx_.lock();
            std::cout<<"Track #"<<_id<<" received Message!!!!! det at: ("<<newDetection->x_mid<<","<<newDetection->y_mid<<")"<< std::endl;
            cout_mtx_.unlock();
        }

        // Prune if track has been coasting too long
        if (_coastedFrames > _maxCoastCount){
            _state = terminated;
            cout_mtx_.lock();
            std::cout<<"Track #"<<_id<<" Terminated."<< std::endl;
            cout_mtx_.unlock();
        }

        cout_mtx_.lock();
        std::cout<<std::endl << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " << std::endl;
        std::cout<<"%%%%%%%%%%% TRACKID: "<<_id<<" ---------> END OF THREAD LOOP " << std::endl;
        std::cout<<"%%%%%%%%%%% _X: "<< std::endl<<_X << std::endl;
        std::cout<<"%%%%%%%%%%% _state: "<<_state << std::endl;
        std::cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " << std::endl << std::endl;
        cout_mtx_.unlock();
    }
}

float TrackedObject::measureDistance(std::shared_ptr<Detection> det){
    return std::sqrt(std::pow((det->x_mid-_X(0)),2)+std::pow((det->y_mid-_X(1)),2));
    }