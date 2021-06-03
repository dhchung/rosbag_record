cmake_minimum_required(VERSION 3.0.2)

project(rosbag_record)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    message_generation
    message_runtime
    rosbag
    std_msgs
    geometry_msgs
    sensor_msgs
)
set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -Wall -g ${CMAKE_CXX_FLAGS}")


catkin_package()

file(GLOB CLASS_SOURCES
    "classes/*")

include_directories(
    classes/
    ${catkin_INCLUDE_DIRS}
)


add_executable(record_by_message
               src/main.cpp
               ${CLASS_SOURCES})

target_link_libraries(record_by_message
                      ${catkin_LIBRARIES})