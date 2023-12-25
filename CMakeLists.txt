cmake_minimum_required(VERSION 3.10)
project(learnOpenGL)

set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile to load")
set(GLAD_OUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Output directory")
set(GLAD_API "gl=3.3" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
option(GLAD_ALL_EXTENSIONS "Include all extensions instead of those specified by GLAD_EXTENSIONS" ON)

# Add GLFW subdirectory
add_subdirectory(include/glfw)
add_subdirectory(include/glad)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add your source files here
set(SOURCE_FILES ./src/main.cpp)

# Add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic")

add_executable(learnOpenGL ${SOURCE_FILES})

# Link libraries
target_link_libraries(learnOpenGL glfw glad ${GLFW_LIBRARIES})