cmake_minimum_required (VERSION 2.6)

project(Q_LEARN)

get_filename_component(Q_DIR ./ ABSOLUTE)
get_filename_component(INC_DIR ./include ABSOLUTE)
get_filename_component(SRC_DIR ./src ABSOLUTE)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -std=c++0x")

include_directories(${INC_DIR})
add_subdirectory(src)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Q_DIR})

add_executable(qlearn_test main.cpp src/QLearner.cpp src/QLearnerNode.cpp src/QLearningSimulate.cpp)


