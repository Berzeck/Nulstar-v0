cmake_minimum_required(VERSION 3.2)
project(Updater)

set(PROJECT_VERSION ${NULSTAR_VERSION})

# Qt5 Settings
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(APP_NAME "${PROJECT_NAME}")
set(APP_DOMAIN "Nulstar")
set(APP_VERSION "0.1.0")
set(APP_VERSION_NAME "OneEye Baby Ninja Egg")
set(APP_ROLE "Role_UpdateManager")
set(APP_ROLE_VERSION "0.1")

set(ENV{LD_LIBRARY_PATH} "${LIBRARIES_OUTPUT_DIR}/Nulstar/0.1.0")
configure_file(${CMAKE_SOURCE_DIR}/AppVersion.h.cmake ${PROJECT_BINARY_DIR}/AppVersion.h)

aux_source_directory(. SOURCE_SRCS)

set(EXECUTABLE_OUTPUT_PATH "${MODULES_OUTPUT_DIR}/Nulstar/${PROJECT_NAME}/${APP_VERSION}")

include_directories(${PROJECT_BINARY_DIR})
include_directories(${LIBRARIES_PATH}/Nulstar/NNetwork)
include_directories(${LIBRARIES_PATH}/Nulstar/NCore)

add_executable(${PROJECT_NAME} ${SOURCE_SRCS})
add_dependencies(${PROJECT_NAME} NCore)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network)
target_link_libraries(${PROJECT_NAME} NCore)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        ${CMAKE_CURRENT_SOURCE_DIR}/Module.ncf ${EXECUTABLE_OUTPUT_PATH}
        )
