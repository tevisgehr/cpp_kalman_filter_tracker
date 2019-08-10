# C++ Kalman Filter Tracker

This repo is a C++ implimentation of a Kalman filter for multiple visual object tracking. The tracker reads in frame-synchronised bounding boxes from an object detector (such as SSD or Faster-RCNN). Each object is tracked asychronously. At each new frame, the tracker manager reads in frame detections and associates them with existing track. Associated detections are sent to corropsonding track threads via an async message queue class. 


## Classes and Structs

### TrackerManager
Methods:
- readNextFrameDetections() - Reads in next set of detections from file (or buffer)
- associate() - Associate new frame detections to TrackedObject instances, via message queue.
- createNewTracks() - Create new TrackedObject instances for unassociated detections. 
- prune() - Kill TrackedObject instances that have been coasting too long or have left the scene.

Data:
- tracks (list<TrackedObjects>) - list of tracked objects
- newDetections (list<Detection>) - Storage for new frame detections.
- shutdown (bool) - Set true to shutdown the tracker.

### TrackedObject
#### Methods:
- simulate()
- timeUpdate()
- measurementUpdate()
- getState() - Getter function used to access the current state estimate. Used for association and visualization. 
#### Data:
- (static) A - State transition matrix

- state (enum State) - 'init', 'active', 'coast', or 'terminated' 
- X (Eigen::MatrixXd (2x2)) - State estimate matrix [[x,y],[v_x,v_y]] 
- P ((Eigen::MatrixXd (2x2)) - Error covatiance matrix

### Detection(struct)
This struct holds the information that has been passed into the program from the detector. Each object represents a single detection.

#### Data:
- x1 (int) - Pixel location of left side of bounding box
- x2 (int) - Pixel location of right side of bounding box
- y1 (int) - Pixel location of top of bounding box
- y2 (int) - Pixel location of bottom of bounding box
- frameID (int) - Frame number

### MessageQueue
This class impliments an asychronous message queue to pass detection objects between the Tracker thread and each of the TrackedObject threads. 

#### Methods:
- send()
- receive()

#### Data:
- frame_detection (Detection)
