add_library(ecs INTERFACE)

target_include_directories(ecs INTERFACE ./include)

target_link_libraries(ecs INTERFACE meta)

add_subdirectory(tests)