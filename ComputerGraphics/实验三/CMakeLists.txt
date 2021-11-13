cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)
set(TARGET_NAME Class3-CameraAndMapPattern)
add_executable(${TARGET_NAME} main.cpp)

# OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_include_directories(${TARGET_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS} )
target_include_directories(${TARGET_NAME} PUBLIC ${GLUT_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY})

#OpenCv
find_package(OpenCV REQUIRED)
target_include_directories(${TARGET_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME}  ${OpenCV_LIBRARIES})



