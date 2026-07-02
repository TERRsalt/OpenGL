cmake_minimum_required(VERSION 4.2)
project(OpenGL CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME} source/main.cpp
    source/files.cpp
    source/fun.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/source/shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/source")