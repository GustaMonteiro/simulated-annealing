cmake_minimum_required(VERSION 3.10)

project(simulated_annealing "CXX")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB sources ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${sources})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

file(COPY ${CMAKE_SOURCE_DIR}/inputs/ DESTINATION ${CMAKE_BINARY_DIR})
