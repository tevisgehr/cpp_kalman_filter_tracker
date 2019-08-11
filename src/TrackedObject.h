#ifndef TRACKED_OBJECT_H
#define TRACKED_OBJECT_H

#include <iostream>
#include <thread>
#include <deque>

#include "Eigen/Dense"

extern std::mutex cout_mtx_;

struct Detection{
    int x1, x2, y1, y2;
    float x_mid, y_mid;
    int frameId;
};

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

class TrackedObject{
public:
    enum State{
        init,
        active,
        coast,
        terminated};

    const int _id;         // Unique constant id for the object

    TrackedObject(std::shared_ptr<Detection>);
    void run(); // Main run loop to be activated in thread started by manager
    std::vector<float> getStateEstimate(); //Getter function returns {x, y, v_x, v_y} for track.
    float measureDistance(std::shared_ptr<Detection>);

    MessageQueue<std::shared_ptr<Detection>> _detectionQueue;
    void sendDetection(std::shared_ptr<Detection> &);

private:
    static int _idCount;   // Static member increments in constructor and ensures unique _id for each object

    static Eigen::MatrixXd _A; //State transition matrix (static)
    static Eigen::MatrixXd _H; //Measurement matrix (static)

    State _state;
    Eigen::MatrixXd _X;
    Eigen::MatrixXd _P;

    void timeUpdate();
    void measurementUpdate();


};

#endif