#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#include "Tracker.h"

TrackerManager::TrackerManager(){}

void TrackerManager::setNewDetections(int idx, std::vector<std::vector<float>> incomingFrameDetections){
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

    for (int i_track = 0; i_track < _tracks.size(); ++i_track){
        auto &track = _tracks[i_track];

        if (track->getState() != terminated){
            cout_mtx_.lock();
            std::cout<<"!!!Track!!!: "<<track->_id<<std::endl;
            cout_mtx_.unlock();
            
            bool track_associated = false;

            for (int i_det = 0; i_det < _newDetections.size(); ++i_det){
                auto &det = _newDetections[i_det];
                cout_mtx_.lock();
                std::cout<<"!!!Detection!!!: "<<det->x_mid<<","<<det->y_mid<<std::endl;
                cout_mtx_.unlock();

                float distance = track->measureDistance(det);
                cout_mtx_.lock();
                std::cout<<"Measured distance: "<< distance <<std::endl;
                cout_mtx_.unlock();

                if (distance <= _assocationDistanceThreshold){
                    det->associated = true;
                    track->sendDetection(det);
                    cout_mtx_.lock();
                    std::cout<<" *****Detection "<<i_det<<" associated to Track " <<i_track<<std::endl;
                    cout_mtx_.unlock();    
                    track_associated = true;
                    break;
                }
            }

            if (!track_associated){
                cout_mtx_.lock();
                std::cout<<" ********Track NOT associated! Sending nullptr..."<<std::endl;
                cout_mtx_.unlock();  
                track->sendDetection(nullptr);
            }
        }
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