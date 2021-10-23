cmake_minimum_required(VERSION 3.20)
project(MedicalImageInhance)

set(TARGET_NAME ${Enhance})
set(CMAKE_CXX_STANDARD 17)



find_package(OpenCV REQUIRED)
add_executable(${TARGET_NAME} PUBLIC main.cpp)
include_directories( ${OpenCV_INCLUDE_DIRS})


target_link_libraries(${TARGET_NAME} PUBLIC ${OpenCV_LIBRARIES})