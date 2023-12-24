cmake_minimum_required(VERSION 3.10)
project(learnOpenGL)

# Add GLFW subdirectory
add_subdirectory(include/glfw)

# Glad command
add_custom_command(
    OUTPUT ./src/glad.c
    COMMENT "Generating GLAD files"
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/include/glad && python3 -m glad --profile core --out-path ${CMAKE_CURRENT_BINARY_DIR} --generator c --api gl=4.6 --spec gl
)

# Include directories
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

# Add your source files here
set(SOURCE_FILES ./src/main.cpp)

add_executable(learnOpenGL ${SOURCE_FILES} ./src/glad.c)

# Link libraries
target_link_libraries(learnOpenGL glfw ${GLFW_LIBRARIES})