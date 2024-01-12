#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

#include "Shader.hpp"
#include "Window.hpp"
#include "Model.hpp"

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processImput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
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
    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -7.0f));
    defaultShader.setMat4("view", view);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(width) / (float)height, 0.1f, 100.0f);
    defaultShader.setMat4("projection", projection);

    try {
        while (glfwWindowShouldClose(window) == GL_FALSE) {
            processImput(window);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT(hicpp-signed-bitwise)

            glm::mat4 model(1.0f);
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            defaultShader.setMat4("model", model);

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