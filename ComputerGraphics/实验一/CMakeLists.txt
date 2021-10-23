#========================================
# cmake最低版本要求
#========================================
cmake_minimum_required(VERSION 3.15)
#========================================
# 项目名称+基本变量设置
#========================================
set(TEST_NAME test)
set(HOMEWORK_NAME homework)
set(CMAKE_CXX_STANDARD 17)

#========================================
# 添加到可执行文件
#========================================
add_executable(${TEST_NAME} test.cpp)

add_executable(${HOMEWORK_NAME} homework.cpp)

# OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_include_directories(${TEST_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS} )
target_include_directories(${TEST_NAME} PUBLIC ${GLUT_INCLUDE_DIRS})
target_link_libraries(${TEST_NAME} ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY})


target_include_directories(${HOMEWORK_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS} )
target_include_directories(${HOMEWORK_NAME} PUBLIC ${GLUT_INCLUDE_DIRS})
target_link_libraries(${HOMEWORK_NAME} ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY})