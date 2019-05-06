add_executable(bomberman
    src/Main.cpp)

target_link_libraries(bomberman log)

set_target_properties(bomberman PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")