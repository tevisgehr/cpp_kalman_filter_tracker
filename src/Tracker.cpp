#include <iostream>
#include <thread>
#include <vector>

#include "Tracker.h"
// #include "cout_mtx_.h"

TrackerManager::TrackerManager(){}

void TrackerManager::setNewDetections(int idx, std::vector<std::vector<int>> incomingFrameDetections){
    // cout_mtx_::m.lock();
    std::cout<<"Running setNewDetections()."<<std::endl;
    // cout_mtx_::m.unlock();

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
    // cout_mtx_::m.lock();
    std::cout<<"Running associate()."<<std::endl;
    // cout_mtx_::m.unlock();
}

void TrackerManager::createNewTracks(){
    // cout_mtx_::m.lock();
    std::cout<<"Running createNewTracks()."<<std::endl;
    // cout_mtx_::m.unlock();

    for (auto &newDet: _newDetections){
        // For each remaining unassociated detection, start a new track
        TrackedObject newTrack(newDet);
        _threads.emplace_back(std::thread(&TrackedObject::run, newTrack));
    }
}

void TrackerManager::prune(){
    // cout_mtx_::m.lock();
    std::cout<<"Running prune()."<<std::endl;
    // cout_mtx_::m.unlock();
}