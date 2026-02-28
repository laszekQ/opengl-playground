cmake_minimum_required(VERSION 4.0)
project(opengl_playground LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ..)

file(GLOB SOURCES
        src/*.cpp
        src/classes/*.cpp
        src/*.c
        include/*.h
)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


add_executable(opengl_playground ${SOURCES}
        include/other/classes/Object.h
        src/classes/Object.cpp)

target_link_libraries(opengl_playground OpenGL::GL)
target_link_libraries(opengl_playground glfw)
target_link_libraries(opengl_playground assimp)
target_link_libraries(opengl_playground imgui)