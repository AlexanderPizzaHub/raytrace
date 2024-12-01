cmake_minimum_required(VERSION 3.10)

project(RayTrace)

set(CMAKE_CXX_STANDARD 11)

set(DESTINATION .)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall -fopt-info-vec-optimized -lstdc++")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3  -Wall -pedantic-errors -lstdc++")

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    /opt/homebrew/Cellar/open-mpi/5.0.5/include
    F:/CODES/Microsoft-MPI-master/src/include
)

link_directories(
    /opt/homebrew/Cellar/open-mpi/5.0.5/include
    F:/CODES/Microsoft-MPI-master/src/include
)

file(
    GLOB SOURCE
    src/*.cpp
    src/*/*.cpp
)

add_executable(
    raytrace
    ${SOURCE}
)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
install(TARGETS raytrace DESTINATION bin)