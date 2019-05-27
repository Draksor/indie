cmake_minimum_required(VERSION 3.9.2)

project(indie)

set(CMAKE_CXX_STANDARD 17)

set(INDIE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(3rdParty)

# Load macros
include("${INDIE_MODULE_PATH}/OutputDirectory.cmake")
include("${INDIE_MODULE_PATH}/AddTest.cmake")

add_subdirectory(meta)
add_subdirectory(log)
add_subdirectory(event)
add_subdirectory(ecs)
add_subdirectory(bomberman)