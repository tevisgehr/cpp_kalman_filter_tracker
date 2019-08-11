#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#include "Tracker.h"

TrackerManager::TrackerManager(){}

void TrackerManager::setNewDetections(int idx, std::vector<std::vector<int>> incomingFrameDetections){
    cout_mtx.lock();
    std::cout<<"Running setNewDetections()."<<std::endl;
    cout_mtx.unlock();

    _newDetections.clear();
    for (auto &det: incomingFrameDetections){
        Detection newDet;
        newDet.frameId = idx;
        newDet.x1 = det[0];
        newDet.x2 = det[1];
        newDet.y1 = det[2];
        newDet.y2 = det[3];
        newDet.x_mid = ((float)(newDet.x1)+(float)(newDet.x2))/2; 
        newDet.y_mid = ((float)(newDet.y1)+(float)(newDet.y2))/2; 
        _newDetections.push_back(std::make_shared<Detection>(newDet));
    }
}

void TrackerManager::associate(){
    cout_mtx.lock();
    std::cout<<"Running associate()."<<std::endl;
    cout_mtx.unlock();

    for (auto det: _newDetections){
            cout_mtx.lock();
            std::cout<<"!!!Detection!!!: "<<det->x_mid<<","<<det->y_mid<<std::endl;
            cout_mtx.unlock();
        for (auto track: _tracks){
            cout_mtx.lock();
            std::cout<<"!!!Track!!!: "<<track->_id<<std::endl;
            cout_mtx.unlock();
            
            float distance = track->measureDistance(det);
            
            cout_mtx.lock();
            std::cout<<"Measured distance: "<< distance <<std::endl;
            cout_mtx.unlock();
        }
    }
}

void TrackerManager::createNewTracks(){
    cout_mtx.lock();
    std::cout<<"Running createNewTracks()."<<std::endl;
    cout_mtx.unlock();

    for (auto &newDet: _newDetections){
        // For each remaining unassociated detection, start a new track
        TrackedObject newTrack(newDet);
        _tracks.push_back(std::make_shared<TrackedObject>(newTrack));
        _threads.emplace_back(std::thread(&TrackedObject::run, newTrack));
    }
}

void TrackerManager::prune(){
    cout_mtx.lock();
    std::cout<<"Running prune()."<<std::endl;
    cout_mtx.unlock();
}