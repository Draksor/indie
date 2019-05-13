add_library(meta INTERFACE)

target_include_directories(meta INTERFACE ./include)

target_link_libraries(meta INTERFACE)

add_subdirectory(tests)