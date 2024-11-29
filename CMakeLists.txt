cmake_minimum_required(VERSION 3.0)

project(RayTrace)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_COMPILER=mpic++)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall -fopt-info-vec-optimized")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -Wall -pedantic-errors")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    /opt/homebrew/Cellar/open-mpi/5.0.5/include
)

link_directories(
    /opt/homebrew/Cellar/open-mpi/5.0.5/include
)

file(
    GLOB SOURCE
    src/*.cpp
)

add_executable(
    raytrace
    ${SOURCE}
)

install(TARGETS raytrace DESTINATION ..)