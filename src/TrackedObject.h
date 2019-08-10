#ifndef TRACKED_OBJECT_H
#define TRACKED_OBJECT_H

#include <iostream>
#include "Eigen/Dense"

struct Detection{
    int x1, x2, y1, y2;
    int frameId;
};

class TrackedObject{
public:
    enum State{
        init,
        active,
        coast,
        terminated};

private:
    State state;
    Eigen::MatrixXd X;

};

#endif