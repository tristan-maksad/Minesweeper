cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(P3Minesweeper)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "C:/Users/trist/OneDrive/Downloads/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)
add_executable(P3Minesweeper
        main.cpp
        WelcomeWindow.h
        WelcomeWindow.cpp
        GameWindow.h
        GameWindow.cpp
)
target_link_libraries(P3Minesweeper
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
)