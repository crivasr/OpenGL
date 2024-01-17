#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

#include "Shader.hpp"
#include "Window.hpp"
#include "Model.hpp"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float yaw   = -90.0f;
float pitch = 0.0f;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback([[maybe_unused]] GLFWwindow *window, double xpos, double ypos) {
    static double lastX;
    static double lastY;

    static bool firstMouse = true;
    if (firstMouse) {
        lastX      = xpos;
        lastY      = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top

    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.2f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += (float)xoffset;
    pitch += (float)yoffset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void processImput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    bool  isRunning   = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    float cameraSpeed = isRunning ? 10.0f * deltaTime : 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraUp;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraUp;
    }
}

int main() {
    GLFWwindow           *window     = nullptr;
    constexpr int         width      = 800;
    constexpr int         height     = 600;
    constexpr const char *windowName = "LearnOpenGL";

    try {
        window = createMainWindow(width, height, windowName);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } catch (const std::string &error) {
        std::cerr << "Error creating window:\n" << error << std::endl;
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    Model  backpack("backpack/backpack.obj");
    Shader defaultShader;

    try {
        defaultShader.add("default.vert", Shader::VERTEX);
        defaultShader.add("default.frag", Shader::FRAGMENT);
        defaultShader.link();
    } catch (const std::runtime_error &error) {
        std::cerr << "Error creating shaders:\n" << error.what() << std::endl;
        return 1;
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    defaultShader.use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(width) / (float)height, 0.1f, 100.0f);
    defaultShader.setMat4("projection", projection);

    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
    defaultShader.setVec3("lightPos", lightPos);

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    defaultShader.setVec3("lightColor", lightColor);

    glm::vec3 ambientColor(0.2f, 0.2f, 0.2f);
    defaultShader.setVec3("ambientColor", ambientColor);

    glm::mat4 model(1.0f);
    defaultShader.setMat4("model", model);

    try {
        while (glfwWindowShouldClose(window) == GL_FALSE) {
            processImput(window);

            auto currentFrame = (float)glfwGetTime();
            deltaTime         = currentFrame - lastFrame;
            lastFrame         = currentFrame;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT(hicpp-signed-bitwise)

            // glm::mat4 model(1.0f);
            // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            // defaultShader.setMat4("model", model);

            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            defaultShader.setMat4("view", view);

            glm::vec3 newLightPos = lightPos;
            newLightPos.x += sinf((float)glfwGetTime()) * 2.0f;
            newLightPos.z += cosf((float)glfwGetTime()) * 2.0f;
            defaultShader.setVec3("lightPos", newLightPos);

            backpack.Draw(defaultShader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } catch (const std::runtime_error &error) {
        std::cerr << "Error running main loop:\n" << error.what() << std::endl;
        return 1;
    }

    backpack.deleteModel();
    defaultShader.deleteShader();

    glfwTerminate();
    return 0;
}