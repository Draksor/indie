add_executable(indie_meta_tests
    TypeList.cpp
    Tuple.cpp)

set_target_properties(indie_meta_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

target_link_libraries(indie_meta_tests gtest gtest_main meta)