# C++ Kalman Filter Tracker

This repo is a C++ implimentation of a Kalman filter for multiple visual object tracking. The tracker reads in frame-synchronised bounding boxes from an object detector (such as SSD or Faster-RCNN). Each object is tracked asychronously. At each new frame, the tracker manager reads in frame detections and associates them with existing tracks. Associated detections are sent to corrosponding track threads via an async message queue class. 

This project was done as my capstone for Udacity's C++ nanodegree program.  

## Installation and Usage
To install, compile and run this repo, follow these steps:
1. Clone this repo.
2. The Kalman filter matrix operations make use of the open source 'Eigen' library. Installing Eigen is simple. First download the source files (http://bitbucket.org/eigen/eigen/get/3.3.7.tar.bz2). Untar the package. Then copy the contained 'Eigen' directory into this repo under src/. This will include all required header files into your project.
3. From project root directory, run the following:
 
``` mkdir build && cd build ```
 
``` cmake .. ```
 
``` make```
 

The traker is now installed an compiled. Run from build by running:
 
``` ./tracker```

## Input Data
Data is read in from a file. Four example files are included in the data/ folder. The choise of input file can be changed inside main.cpp at compile-time. The format for data files is as follows:

- Each row represents all of the bounding box detection from a single frame. Each row consists of multiples of four floats. The floats corrospond to the ( x1, x2, y1, y2 ) coordiates of the detection bounding boxes. Example:

row 1: 10.0 20.0 50.0 60.0                        (frame 1: one detection at x1: 10.0, x2: 20.0, y1: 50.0 y2:60.0) 
 
row 2:                                            (frame 2: no detections) 
 
row 3: 11.0 21.0 51.0 61.0 35.5 46.6 78.9 99.8    (frame 3: two detections) 
 
... 



## Classes and Structs

### TrackerManager
#### Methods:
- setNewDetections() - Reads in next set of detections from file (or internal buffer)
- associate() - Associate new frame detections to TrackedObject instances, via message queue.
- createNewTracks() - Create new TrackedObject instances for unassociated detections. 
- prune() - Kill TrackedObject instances that have been coasting too long or have left the scene.

#### Data:
- _threads (std::vector< std::thread >) - Vector of threads for each TrackedObject instance
- _tracks (list< shared_ptr<TrackedObjects >) - list of tracked objects
- _newDetections (list< shared_ptr<Detection> >) - Temporary storage for new frame detections
- _shutdown (bool) - Set true to shutdown the tracker
 
Settings (constants) :
- _assocationDistanceThreshold (float) - max distance between a track state and new detection that will resut in a positive association.

### TrackedObject
#### Methods:
- run() - Main loop run by thread for each track
- timeUpdate() - Time update (prediction step of Kalman filter)
- measurementUpdate() - Measurement update (correction step of Kalman filter)
- getStateEstimate() - Getter function used to access the current state estimate. Used for association and visualization. 
- measureDistance() - Returns the distance between its current location estimate and an incoming Detection object.
- sendDetection() - Sends a Detection object to the message queue for this object
#### Data:
- _A Eigen::MatrixXd (6x6)) - State transition matrix (static) 
- _H Eigen::MatrixXd (2x6)) - Measurement matrix (static) 
- _state (enum State) - 'init', 'active', 'coast', or 'terminated' 
- _coastedFrames (int) - number of frames since the tracked object has had a positive measurement (detection) association
- _X (Eigen::MatrixXd (6x1)) - State estimate vector [x,y,v_x,v_y,a_x,a_y] 
- _P (Eigen::MatrixXd (6x6)) - Error covariance matrix
- _R (Eigen::MatrixXd (2x2)) - Measurement covariance matrix
- _Q (Eigen::MatrixXd (6x6)) - Process covariance matrix
- _Z (Eigen::MatrixXd (2x1)) - Measurement vector
- _detectionQueue ( MessageQueue< std::shared_ptr<Detection> > ) - Each TrackedObject instance has its own MessageQueue instance for communicating detections between main  thread and track thread

Settings (constants):
- _maxCoastCount (int) - number of coasted frames allowed before a track is killed
- _initalErrorCovariance - inital value for diagonal entries in error covariance matrix
-  _processVariance - Assumed system process variance
-  _measurmantVariance - Assumed system measurement variance

### Detection(struct)
This struct holds the information that has been passed into the program from the detector. Each object represents a single detection.

#### Data:
- x1 (int) - Pixel location of left side of bounding box
- x2 (int) - Pixel location of right side of bounding box
- y1 (int) - Pixel location of top of bounding box
- y2 (int) - Pixel location of bottom of bounding box
- x_mid (float) - x location of the mid-point of the detection bounding box
- y_mid (float) - y location of the mid-point of the detection bounding box
- frameID (int) - Frame number
- associated (bool) - False by default. Whether or not the detection has been associated with a track.
### MessageQueue
This class impliments an asychronous message queue to pass detection objects between the Tracker thread and each of the TrackedObject threads. 

#### Methods:
- send()
- receive()

#### Data:
- _mutex (std::mutex)
- _cond (std::condition_variable)
- _queue (std::deque<T>)


## Rubric Points
- "The project demonstrates an understanding of C++ functions and control structures." - Throughout program
- "The project reads data from a file and process the data, or the program writes data to a file." - main.cpp:33
- "The project uses Object Oriented Programming techniques." - Throughout program
- "Classes use appropriate access specifiers for class members." - TrackedObject.h:38-82
- "Class constructors utilize member initialization lists." - TrackedObject.cpp:83
- "Classes abstract implementation details from their interfaces." - Throughout program
- "Classes encapsulate behavior." - Throughout program
- "Templates generalize functions in the project." - TrackedObject.cpp:25
- "The project uses move semantics to move data, instead of copying it, where possible." - TrackedObject.cpp:80
- "The project uses smart pointers instead of raw pointers." - Throughout program
- "The project uses multithreading." - Throughout program
- "A mutex or lock is used in the project." - Throughout program
- "A condition variable is used in the project." - TrackedObject.h:34

** Important Note: Several examples from the C++ nanodegree coursework were adapted for this project. In particular the message queue object, as covered in the concurrencey course, was adapted. I therefore give credit to Udacity for the code that impliments the MessageQueue class, as I made only minor modifications. Furthermore, I use the Eigen libraries for matrix math, which are open source. **    