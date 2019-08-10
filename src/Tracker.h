#ifndef TRACKER_H
#define TRACKER_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include <list>

#include "TrackedObject.h"

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
    bool shutdown = false;
    void associate();
private:
    std::list<TrackedObject> tracks;
};

#endif