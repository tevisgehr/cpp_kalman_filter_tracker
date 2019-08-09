# C++ Kalman Filter Tracker

This repo is a C++ implimentation of a Kalman filter for multiple visual object tracking. The tracker reads in time-stamped bounding boxes from an object detector (such as SSD or Faster-RCNN). Each object is tracked asychronously.


## Classes

#### Tracker
Methods:

Data:

#### TrackedObject
Methods:

Data:

#### Detection
Methods:
- None

Data:
- x1 (int) - Pixel location of left side of bounding box
- x2 (int) - Pixel location of right side of bounding box
- y1 (int) - Pixel location of top of bounding box
- y2 (int) - Pixel location of bottom of bounding box



