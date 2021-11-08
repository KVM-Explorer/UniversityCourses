#========================================
# cmake最低版本要求
#========================================
cmake_minimum_required(VERSION 3.15)
#========================================
# 项目名称+基本变量设置
#========================================
set(TARGET_NAME test2)
set(CMAKE_CXX_STANDARD 17)

#========================================
# 添加到可执行文件
#========================================
add_executable(${TARGET_NAME} main.cpp)

# OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_include_directories(${TARGET_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS} )
target_include_directories(${TARGET_NAME} PUBLIC ${GLUT_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY})


