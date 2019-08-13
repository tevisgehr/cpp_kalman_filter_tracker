#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#include "Tracker.h"

TrackerManager::TrackerManager(){}

void TrackerManager::setNewDetections(int idx, std::vector<std::vector<int>> incomingFrameDetections){
    cout_mtx_.lock();
    std::cout<<"Running setNewDetections()."<<std::endl;
    cout_mtx_.unlock();

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
    cout_mtx_.lock();
    std::cout<<"Running associate()."<<std::endl;
    cout_mtx_.unlock();
    int i = 0;
    for (auto &det: _newDetections){
            cout_mtx_.lock();
            std::cout<<"!!!Detection!!!: "<<det->x_mid<<","<<det->y_mid<<std::endl;
            cout_mtx_.unlock();
        int j = 0;
        for (auto &track: _tracks){
            track->_associated = false;
            cout_mtx_.lock();
            std::cout<<"!!!Track!!!: "<<track->_id<<std::endl;
            cout_mtx_.unlock();
            
            float distance = track->measureDistance(det);
            cout_mtx_.lock();
            std::cout<<"Measured distance: "<< distance <<std::endl;
            cout_mtx_.unlock();

            if (distance <= _assocationDistanceThreshold){
                det->associated = true;
                track->_associated = true;
                track->sendDetection(det);
                cout_mtx_.lock();
                std::cout<<" ********Detection associated! "<<std::endl;
                cout_mtx_.unlock();    
                break;

            }

            ++j;
        }
        ++i;
    }
}

void TrackerManager::createNewTracks(){
    cout_mtx_.lock();
    std::cout<<"Running createNewTracks()."<<std::endl;
    cout_mtx_.unlock();

    for (auto &newDet: _newDetections){
        // For each remaining unassociated detection, start a new track
        if (!newDet->associated){
            std::shared_ptr<TrackedObject> newTrack = std::make_shared<TrackedObject>(newDet);
            _tracks.push_back(newTrack);
            _threads.emplace_back(std::thread(&TrackedObject::run, newTrack));
        }
    }
}

void TrackerManager::prune(){
    cout_mtx_.lock();
    std::cout<<"Running prune()."<<std::endl;
    cout_mtx_.unlock();
}