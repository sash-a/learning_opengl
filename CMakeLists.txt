cmake_minimum_required(VERSION 3.9)
project(ogl_final)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
include_directories(include/GL)

add_executable(ogl_final
    build/geometry.o
    build/glew32.dll
    build/glwindow.o
    build/main.o
    build/prac1
    build/SDL2.dll
    build/simple.frag
    build/simple.vert
    include/GL/glew.h
    include/GL/glxew.h
    include/GL/wglew.h
    src/geometry.cpp
    src/geometry.h
    src/glwindow.cpp
    src/glwindow.h
    src/main.cpp
    Makefile src/Gameobject.cpp src/Gameobject.h src/InputHandler.cpp src/InputHandler.h)
