#ifndef TRACKER_H
#define TRACKER_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include <list>
#include <thread>

#include "TrackedObject.h"

extern std::mutex cout_mtx_;

class TrackerManager{
public:
    TrackerManager();
    bool _shutdown = false;

    void setNewDetections(int, std::vector<std::vector<int>>);
    void associate();
    void createNewTracks();

    // ################### Settings ###################
    const float _assocationDistanceThreshold = 15;
    // ################################################

private:
    std::vector<std::thread> _threads;                       // Threads for the TrackedObjects to run in
    std::vector<std::shared_ptr<TrackedObject>> _tracks;       // vector of shared_pts to tracks
    std::vector<std::shared_ptr<Detection>> _newDetections;    // vector of shared_pts to detections
};

#endif