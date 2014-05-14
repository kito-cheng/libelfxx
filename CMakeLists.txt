cmake_minimum_required (VERSION 2.6)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/libelf++)
set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -Wall -g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -Wall -O3")

add_subdirectory(src)
