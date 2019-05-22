add_library(ecs INTERFACE)

target_include_directories(ecs INTERFACE ./include)

target_link_libraries(ecs INTERFACE meta)

ADD_TEST(indie_ecs_tests tests ecs)