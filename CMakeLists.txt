cmake_minimum_required(VERSION 3.10)
project(learnOpenGL)

set(CMAKE_CXX_STANDARD 20)

# Set GLAD options
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile to load")
set(GLAD_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE STRING "Output directory")
set(GLAD_API "gl=3.3" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
option(GLAD_ALL_EXTENSIONS "Include all extensions instead of those specified by GLAD_EXTENSIONS" ON)

# Add dependencies
add_subdirectory(include/glfw)
add_subdirectory(include/glad)
add_subdirectory(include/glm)
add_subdirectory(include/assimp)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add source files
set(SOURCE_FILES 
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/Shader.cpp"
    "${CMAKE_SOURCE_DIR}/src/Window.cpp"
    "${CMAKE_SOURCE_DIR}/src/Mesh.cpp"
    "${CMAKE_SOURCE_DIR}/src/Model.cpp"
    "${CMAKE_SOURCE_DIR}/src/fs_helpers.cpp"
)

# Add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra")

# Add executable
add_executable(learnOpenGL ${SOURCE_FILES})

# Copy shaders to resources folder
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/src/resources"
        "${CMAKE_BINARY_DIR}/resources"
    COMMENT "Copying resources to build folder"
)

add_dependencies(learnOpenGL copy_resources)

# Link libraries
target_link_libraries(learnOpenGL glfw glad assimp glm)