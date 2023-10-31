cmake_minimum_required(VERSION 3.26)
project(OpenCVTest)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(OpenCVTest PRIVATE ${OpenCV_LIBS})