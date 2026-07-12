cmake_minimum_required(VERSION 4.2)
project(OpenGL CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

include(FetchContent)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb)

add_executable(${PROJECT_NAME} source/_MAIN_.cpp
    source/files.cpp

    source/keyboard.cpp

    source/random.cpp

    #minor # shaders/ #

    source/shaders/renderer.cpp
    source/shaders/texture.cpp

    #minor # _vendor_/ #

    source/_vendor_/stb_image.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${stb_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
)

#exp # Only for final compilation #
#file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
#file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/source/shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/source")