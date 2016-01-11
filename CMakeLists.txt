# set(CMAKE_VERBOSE_MAKEFILE 1)
cmake_minimum_required(VERSION 2.8) # same as vaa3d
project(Vaa3D-SIGEN)

option(BUILD_MAIN "Build runner program." OFF)

if(BUILD_MAIN)
  find_package(PkgConfig)
  pkg_check_modules(OPENCV REQUIRED opencv)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Woverloaded-virtual -Wnon-virtual-dtor")

add_subdirectory(src)
add_subdirectory(test)
