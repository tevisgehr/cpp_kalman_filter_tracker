cmake_minimum_required(VERSION 3.11.3)

# set(CMAKE_CXX_STANDARD 17)
project(tracker)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

######### ADD OPENCV later for graphics ################
# find_package(OpenCV 4.1 REQUIRED)
# include_directories(${OpenCV_INCLUDE_DIRS})
# link_directories(${OpenCV_LIBRARY_DIRS})
# add_definitions(${OpenCV_DEFINITIONS})
########################################################

# Find all executables
file(GLOB project_SRCS src/*.cpp) #src/*.h

# Add project executable
add_executable(tracker ${project_SRCS})

######### ADD OPENCV later for graphics #################
# target_link_libraries(traffic_simutrackerlation ${OpenCV_LIBRARIES})
#########################################################