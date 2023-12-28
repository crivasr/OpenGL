#include "Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

GLFWwindow* createMainWindow(int width, int height, const char* window_name) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(width, height, window_name, nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::string("createMainWindow | Failed to create GLFW window\n");
    }

    glfwMakeContextCurrent(window);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != GL_TRUE) {
        glfwTerminate();
        throw std::string("createMainWindow | Failed to initialize GLAD");
    }

    return window;
}