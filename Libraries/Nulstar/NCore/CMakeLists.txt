cmake_minimum_required(VERSION 3.2)
project(NCore)
aux_source_directory(. SOURCE_SRCS)

set(PROJECT_VERSION ${NULSTAR_VERSION})

# Qt5 Settings
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5WebSockets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${LIBRARIES_PATH}/Nulstar/NNetwork)
set(LIBRARY_OUTPUT_PATH  "${LIBRARIES_OUTPUT_DIR}/Nulstar/${PROJECT_VERSION}")

add_library(${PROJECT_NAME} SHARED ${SOURCE_SRCS})
add_dependencies(${PROJECT_NAME} NNetwork)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::WebSockets)
target_link_libraries(${PROJECT_NAME} NNetwork)