#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>

# include "Tracker.h"

std::vector<std::vector<int>> readDataFile(std::string filepath){
    std::vector<std::vector<int>> data;
    std::ifstream file(filepath);
    std::string line;
    while(std::getline(file, line))
    {
        std::vector<int>   lineData;
        std::stringstream  lineStream(line);
        int value;
        while(lineStream >> value)
        {
            lineData.push_back(value);
        }
        data.push_back(lineData);
    }
    return data;
}

int main(){
    // Open connection to file, read read detections into 'data' vector
    std::string filepath = "../data/test_data2.txt";
    std::vector<std::vector<int>> data = readDataFile(filepath);

    // //print out data vector --TEST ONLY ########################
    // for (auto row : data){
    //     for (auto num : row ){
    //         std::cout << num << ' ';
    //     }
    //     std::cout<<std::endl;
    // }
    // // #########################################################
        


    // Instantiate TrackerManager
    TrackerManager manager;

    // Main loop
    int idx = 0;
    while (!manager.shutdown){

        // 1. Read in frame detections
        std::vector<std::vector<int>> frameDetections;
        int numDetections = data[idx].size() / 4;
        std::cout<<"Frame idx:"<< idx << " numDetections:"<< numDetections <<std::endl;
        for (int n = 0; n < numDetections; ++n){
            std::vector<int> det;
            for (int j = 0; j < 4; ++j){
                det.push_back(data[idx][(4*n) + j]);
            }
            frameDetections.push_back(det);
        }

        //print out frameDetections vector --TEST ONLY ########################
        for (auto det : frameDetections){
            for (auto num : det ){
                std::cout << num << ' ';
            }
            std::cout<<std::endl;
        }
        // #########################################################

        // 2. Associate detections (measurements) to existing tracks

        // 3. Create new tracks from unassociated measurements

        // 4. Prune dead tracks

        // 5. Update visuals 

    
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        ++idx;
    }
}