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
    bool associated = false;
};

enum State{
    init,
    active,
    coast,
    terminated};

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
    const int _id;         // Unique constant id for the object

    // ################### Settings ###################
    const int _maxCoastCount = 3;
    const float _initalErrorCovariance = 1.0;
    const float _processVariance = 1.0;
    const float _measurmantVariance = 1.0;
    // ################################################

    TrackedObject(std::shared_ptr<Detection>);
    void run(); // Main run loop to be activated in thread started by manager
    std::vector<float> getStateEstimate(); //Getter function returns {x, y, v_x, v_y} for track.
    State getState(){return _state;}
    float measureDistance(std::shared_ptr<Detection>);

    MessageQueue<std::shared_ptr<Detection>> _detectionQueue;
    void sendDetection(std::shared_ptr<Detection>);

private:
    static int _idCount;   // Static member increments in constructor and ensures unique _id for each object

    static Eigen::Matrix<float, 6, 6> _A; //State transition matrix (static)
    static Eigen::Matrix<float, 2, 6> _H; //Measurement matrix (static)

    State _state = init;
    int _coastedFrames = 0;

    // State estimate vector
    Eigen::Matrix<float, 6, 1> _X;

    // Error covariance matrix
    Eigen::Matrix<float, 6, 6> _P;
    
    // Measurement covariance matrix 
    Eigen::Matrix<float, 2, 2> _R;

    // Process covariance matrix
    Eigen::Matrix<float, 6, 6> _Q;

    // Measurement vector
    Eigen::Matrix<float, 2, 1> _Z;

    void timeUpdate();
    void measurementUpdate();

};

#endif