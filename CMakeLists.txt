cmake_minimum_required(VERSION 3.16)
project(blickfeld-qb2 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default options
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Detect if project is used with FetchContent
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(BF_BUILD_EXAMPLES "Build examples" ON)
else()
    option(BF_BUILD_EXAMPLES "Build examples" OFF)
endif()

# Main target
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE
    ${PROJECT_NAME}-protocol
    ${PROJECT_NAME}-cpp
)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME})

# Data
add_subdirectory(data)

# Protocol
add_subdirectory(protocol)

# C++
add_subdirectory(cpp)

# CMake Package
add_subdirectory(cmake)