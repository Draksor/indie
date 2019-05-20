add_executable(indie_ecs_tests
    SparseSet.cpp
    Pool.cpp
    EntityManager.cpp
    System.cpp)

set_target_properties(indie_ecs_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

target_link_libraries(indie_ecs_tests gtest gtest_main ecs)