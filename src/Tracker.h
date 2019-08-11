#ifndef TRACKER_H
#define TRACKER_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include <list>
#include <thread>

#include "TrackedObject.h"

extern std::mutex cout_mtx;

template <class T>
class MessageQueue
{
public:
    T receive();
    void send(T &&msg);

private:
    std::mutex _mutex;
    std::condition_variable _cond;
    std::deque<T> _queue;
};

class TrackerManager{
public:
    TrackerManager();
    bool shutdown = false;
    void associate();
    void createNewTracks();
    void prune();
    void setNewDetections(int, std::vector<std::vector<int>>);

private:
    std::vector<std::thread> _threads;                       // Threads for the TrackedObjects to run in
    std::list<std::shared_ptr<TrackedObject>> _tracks;       // List of shared_pts to tracks. List instead of vector to support easy Insertion and Deletion
    std::list<std::shared_ptr<Detection>> _newDetections;    // List of shared_pts to detections

};

#endif