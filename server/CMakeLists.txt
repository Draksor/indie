add_executable(server src/Main.cpp)

target_link_libraries(server boost_asio log meta)

target_include_directories(server PRIVATE include/)