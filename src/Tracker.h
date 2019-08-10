#ifndef TRACKER_H
#define TRACKER_H

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
};

#endif