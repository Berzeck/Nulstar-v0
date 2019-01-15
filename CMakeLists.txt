cmake_minimum_required(VERSION 3.2)
project(Nulstar)

set(CMAKE_CXX_STANDARD 14)
#SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
#set(CMAKE_MACOSX_RPATH 0)
set(LIBRARIES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Libraries")
set(LIBRARIES_OUTPUT_DIR ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/Libraries)
set(MODULES_OUTPUT_DIR ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/Modules)

add_subdirectory(Libraries)
add_subdirectory(Modules)
