add_executable(bomberman
    src/Main.cpp
    src/Window.cpp
    src/Game.cpp)

target_link_libraries(bomberman irrlicht log ecs)

target_include_directories(bomberman PRIVATE include)