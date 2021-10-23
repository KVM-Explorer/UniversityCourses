cmake_minimum_required(VERSION 3.20)
project(ColorMapping)

set(CMAKE_CXX_STANDARD 17)
set(TARGET_NAME ColorMapping)
add_executable(${TARGET_NAME} main.cpp)



find_package(OpenCV REQUIRED)
target_include_directories(${TARGET_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PUBLIC ${OpenCV_LIBRARIES})