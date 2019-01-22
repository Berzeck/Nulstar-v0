cmake_minimum_required(VERSION 3.2)
project(NNetwork)

set(PROJECT_VERSION ${NULSTAR_VERSION})

# Qt5 Settings
find_package(Qt5Core REQUIRED)
find_package(Qt5WebSockets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(LIBRARY_OUTPUT_PATH  "${LIBRARIES_OUTPUT_DIR}/Nulstar/${PROJECT_VERSION}")

aux_source_directory(. SOURCE_SRCS)

add_library(${PROJECT_NAME} SHARED ${SOURCE_SRCS})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::WebSockets)

enable_testing()

add_executable("${PROJECT_NAME}-unittest" unittest/NMessage_unittest.cpp)

include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries("${PROJECT_NAME}-unittest" ${PROJECT_NAME} gtest gtest_main pthread)
add_test(NAME "${PROJECT_NAME}-unittest" COMMAND "${PROJECT_NAME}-unittest")
add_dependencies("${PROJECT_NAME}-unittest" googletest)
