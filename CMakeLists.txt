cmake_minimum_required(VERSION 3.26)
set(GAME "kaimicraft")
project(${GAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Dynamically build
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# GLFW
add_subdirectory(thirdparty/glfw EXCLUDE_FROM_ALL)

# GLEW
set(GLEW_SHARED ON CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/glew/build/cmake EXCLUDE_FROM_ALL)

if(TARGET libglew_shared)
    add_library(GLEW::GLEW ALIAS libglew_shared)
elseif(TARGET libglew_static)
    add_library(GLEW::GLEW ALIAS libglew_static)
elseif(TARGET glew_s)
    add_library(GLEW::GLEW ALIAS glew_s)
elseif(TARGET glew)
    add_library(GLEW::GLEW ALIAS glew)
else()
    message(FATAL_ERROR "GLEW target not found! Check your GLEW subdirectory and build.")
endif()

set(SRC_FILES
    src/main.cpp
    src/logo.cpp
    src/app.cpp
    src/windowmanager.cpp
    src/inputmanager.cpp
    src/timemanager.cpp
    src/renderer.cpp
    src/lightmanager.cpp
    src/mesh.cpp
    src/shader.cpp
    src/debug.cpp
    src/mouse.cpp
    src/keyboard.cpp
    src/camera.cpp
    src/util.cpp
    src/texturemanager.cpp
    src/tile.cpp
    src/stb_image_impl.cpp
    src/chunkmanager.cpp
    src/terraingenerator.cpp
    src/perlinnoise.cpp
)

add_executable(${GAME} ${SRC_FILES})

target_include_directories(${GAME} PRIVATE
    thirdparty/glm
    thirdparty/stb
    thirdparty/glew/include
)

target_link_libraries(${GAME} PRIVATE
    glfw
    GLEW::GLEW
)

if(UNIX AND NOT APPLE)
    target_link_libraries(${GAME} PRIVATE
        GL
        X11
        pthread
        Xi
        dl
        Xrandr
    )
endif()

# flags
target_compile_options(${GAME} PRIVATE -Wall -Wextra -Wpedantic)

# MacOS rpaths
if(APPLE)
    set_target_properties(${GAME} PROPERTIES
        BUILD_RPATH "@loader_path"
        INSTALL_RPATH "@loader_path"
    )
endif()

set_target_properties(${GAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

message(STATUS "Build type:  ${CMAKE_BUILD_TYPE}")
message(STATUS "Build shared libraries:  ${BUILD_SHARED_LIBS}")
