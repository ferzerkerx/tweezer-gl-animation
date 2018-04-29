cmake_minimum_required(VERSION 3.10)
project(tweezer)

set(CMAKE_CXX_STANDARD 11)

add_executable(tweezer src/main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(tweezer ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )