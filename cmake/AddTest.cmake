macro(ADD_TEST target_name directory lib)
    set(sources)

    # Get all source files of directory variable and put them into sources variable
    aux_source_directory(${directory} sources)

    message(STATUS "Add new test: ${target_name}")

    add_executable(${target_name} ${sources})

    target_link_libraries(${target_name} gtest gtest_main ${lib})
endmacro()