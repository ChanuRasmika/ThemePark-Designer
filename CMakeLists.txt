cmake_minimum_required(VERSION 3.10)
project(ThemeParkBuilder)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source files
file(GLOB SRC_FILES
    src/main.cpp
    src/renderer.cpp
    src/simple3d.cpp
    src/stall.cpp
    src/imgui_setup.cpp
    src/*.c
    src/algorithms/*.cpp
)

# ImGui source files (add these to your project)
set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl2.cpp
)

# Executable target
add_executable(ThemeParkBuilder ${SRC_FILES} ${IMGUI_SOURCES})

# Include directories
target_include_directories(ThemeParkBuilder PRIVATE 
    ${CMAKE_SOURCE_DIR}/include  # Add the root 'include' directory
    ${CMAKE_SOURCE_DIR}/include/algorithms  # Add the 'algorithms' directory inside 'include'
    ${CMAKE_SOURCE_DIR}/imgui  # ImGui main directory
    ${CMAKE_SOURCE_DIR}/imgui/backends  # ImGui backends
)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Link libraries to the executable
target_link_libraries(ThemeParkBuilder
    OpenGL::GL
    glfw
)
