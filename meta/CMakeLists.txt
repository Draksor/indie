add_library(meta INTERFACE)

target_include_directories(meta INTERFACE ./include)

target_link_libraries(meta INTERFACE)

ADD_TEST(indie_meta_tests tests ecs)