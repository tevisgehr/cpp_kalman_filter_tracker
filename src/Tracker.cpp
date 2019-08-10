#include <iostream>

#include "Tracker.h"

TrackerManager::TrackerManager(){}

void TrackerManager::associate(){
    std::cout<<"Running associate()."<<std::endl;
}

void TrackerManager::createNewTracks(){
    std::cout<<"Running createNewTracks()."<<std::endl;
}

void TrackerManager::prune(){
    std::cout<<"Running prune()."<<std::endl;
}