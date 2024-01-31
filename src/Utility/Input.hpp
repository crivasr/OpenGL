#pragma once
#include "OpenGlHeaders.hpp"

class Input {
    private:
    static GLFWwindow *m_window;

    public:
    static void Init(GLFWwindow *window) { Input::m_window = window; }
    static bool isKeyPressed(int key) { return glfwGetKey(m_window, key) == GLFW_PRESS; }
};