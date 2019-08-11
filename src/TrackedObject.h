#ifndef TRACKED_OBJECT_H
#define TRACKED_OBJECT_H

#include <iostream>
#include <thread>

#include "Eigen/Dense"

extern std::mutex cout_mtx;

struct Detection{
    int x1, x2, y1, y2;
    float x_mid, y_mid;
    int frameId;
};

class TrackedObject{
public:
    enum State{
        init,
        active,
        coast,
        terminated};

    TrackedObject(std::shared_ptr<Detection>);
    void run(); // Main run loop to be activated in thread started by manager
    std::vector<float> getStateEstimate(); //Getter function returns {x, y, v_x, v_y} for track.

private:
    static int _idCount;   // Static member increments in constructor and ensures unique _id for each object
    const int _id;         // Unique constant id for the object

    static Eigen::MatrixXd A; //State transition matrix (static)
    static Eigen::MatrixXd H; //Measurement matrix (static)

    State state;
    Eigen::MatrixXd X;
    Eigen::MatrixXd P;

    void timeUpdate();
    void measurementUpdate();


};

#endif