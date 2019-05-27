add_library(event INTERFACE)

target_include_directories(event INTERFACE ./include)

target_link_libraries(event INTERFACE meta)

ADD_TEST(indie_event_tests tests event)