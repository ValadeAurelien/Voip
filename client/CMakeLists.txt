cmake_minimum_required(VERSION 2.8.11)

project(mytest)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Network)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
#set(CMAKE_AUTOMOC ON)

set(CXX_FLAGS "-std=c++11 -Wall")

# Includes directory
set(CLIENT_INC_DIR ${PROJECT_SOURCE_DIR}/includes)
set(COMMON_INC_DIR ${PROJECT_SOURCE_DIR}/../common/includes)
include_directories(${CLIENT_INC_DIR} ${COMMON_INC_DIR})

# Sources directories
add_subdirectory(src)

