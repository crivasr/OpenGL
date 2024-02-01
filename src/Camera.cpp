#include "Camera.hpp"

#include <GLFW/glfw3.h>
#include <Utility/OpenGlHeaders.hpp>
#include <Utility/Input.hpp>

Camera::Camera(GLFWwindow *window) : m_window(window) {
    m_projection = glm::perspective(glm::radians(m_fov), m_width / m_height, m_near, m_far);
    m_view       = glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::update() {
    processKeyboard();
    processMouse();
}

void Camera::processKeyboard() {
    glm::vec3 rotated_front = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
                              glm::vec4(m_front, 1.0f);

    float speed = Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT) ? m_fast_speed : m_slow_speed;

    if (Input::isKeyPressed(GLFW_KEY_W)) {
        m_position += rotated_front * speed;
    }
    if (Input::isKeyPressed(GLFW_KEY_S)) {
        m_position -= rotated_front * speed;
    }
    if (Input::isKeyPressed(GLFW_KEY_A)) {
        m_position -= glm::normalize(glm::cross(rotated_front, m_up)) * speed;
    }
    if (Input::isKeyPressed(GLFW_KEY_D)) {
        m_position += glm::normalize(glm::cross(rotated_front, m_up)) * speed;
    }
    if (Input::isKeyPressed(GLFW_KEY_SPACE)) {
        m_position += m_up * speed;
    }
    if (Input::isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
        m_position -= m_up * speed;
    }

    m_view = glm::lookAt(m_position, m_position + rotated_front, m_up);
}

void Camera::processMouse() {
    static double lastX = m_width / 2.0f;
    static double lastY = m_height / 2.0f;

    double xpos = 0;
    double ypos = 0;

    glfwGetCursorPos(m_window, &xpos, &ypos);

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    xoffset *= m_sensitivity;
    yoffset *= m_sensitivity;

    m_yaw -= (float)xoffset;
    m_pitch -= (float)yoffset;

    if (m_pitch > 89.0f) {
        m_pitch = 89.0f;
    }
    if (m_pitch < -89.0f) {
        m_pitch = -89.0f;
    }

    m_rotation = glm::vec3(m_pitch, m_yaw, 0.0f);
}