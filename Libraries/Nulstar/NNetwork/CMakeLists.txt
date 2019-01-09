cmake_minimum_required(VERSION 3.2)
project(NNetwork)
aux_source_directory(. SOURCE_SRCS)

set(PROJECT_VERSION ${NULSTAR_VERSION})

# Qt5 Settings
find_package(Qt5Core REQUIRED)
find_package(Qt5WebSockets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(LIBRARY_OUTPUT_PATH  "${LIBRARIES_OUTPUT_DIR}/Nulstar/${PROJECT_VERSION}")

add_library(${PROJECT_NAME} SHARED ${SOURCE_SRCS})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::WebSockets)