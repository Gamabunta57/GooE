cmake_minimum_required(VERSION 3.28)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  message(FATAL_ERROR "Prevented in-tree build. Please create a build directory outside of the source code and run \"cmake -S ${CMAKE_SOURCE_DIR} -B .\" from there")
endif()

project(GooE
    VERSION 0.1.0.0
    DESCRIPTION "A Game Engine written in C"
    HOMEPAGE_URL "https://github.com/Gamabunta57/GooE"
    LANGUAGES C)

set(C_STANDARD "C90")
set(C_STANDARD_REQUIRED TRUE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message(STATUS "Building project in '${CMAKE_BUILD_TYPE}' mode")

set(DESTINATION_EXECUTABLE_FOLDER $<1:${PROJECT_SOURCE_DIR}>/output/bin/$<1:${CMAKE_BUILD_TYPE}>)
set(DESTINATION_LIB_FOLDER $<1:${PROJECT_SOURCE_DIR}>/output/libs/$<1:${CMAKE_BUILD_TYPE}>)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DESTINATION_EXECUTABLE_FOLDER})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DESTINATION_LIB_FOLDER})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DESTINATION_LIB_FOLDER})

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(src)
