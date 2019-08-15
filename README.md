# C++ Kalman Filter Tracker

This repo is a C++ implimentation of a Kalman filter for multiple visual object tracking. The tracker reads in frame-synchronised bounding boxes from an object detector (such as SSD or Faster-RCNN). Each object is tracked asychronously. At each new frame, the tracker manager reads in frame detections and associates them with existing track. Associated detections are sent to corropsonding track threads via an async message queue class. 


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
