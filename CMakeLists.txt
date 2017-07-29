cmake_minimum_required(VERSION 3.8.2)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
project(mytest)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Network)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
#set(CMAKE_AUTOMOC ON)

set (CMAKE_CXX_STANDARD 11)
set(CXX_FLAGS "-Wall")

# Includes directory
set(INC_DIR ${PROJECT_SOURCE_DIR}/includes)
include_directories(${INC_DIR})

# Sources directories
add_subdirectory(src)

