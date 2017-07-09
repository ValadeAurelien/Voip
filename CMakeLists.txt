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

# Includes directory
set(INC_DIR ${PROJECT_SOURCE_DIR}/includes)
include_directories(${INC_DIR})

# Sources directories
add_subdirectory(src)

