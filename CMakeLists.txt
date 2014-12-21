cmake_minimum_required (VERSION 2.8.9)
project (highflyers-protocol)

set(CMAKE_CXX_FLAGS "-std=c++0x -D_GLIBCXX_USE_NANOSLEEP -Wall -pedantic ${CMAKE_CXX_FLAGS}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${highflyers-protocol_SOURCE_DIR}/build/libs) 
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${highflyers-protocol_SOURCE_DIR}/build/libs)

add_subdirectory (src) 

