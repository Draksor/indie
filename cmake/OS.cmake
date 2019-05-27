# This module sets LINUX to true if the building os is linux.
# LINUX is not set by default by CMake.

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()