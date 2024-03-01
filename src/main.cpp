#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <memory>

#include <Camera.hpp>
#include <Shader.hpp>
#include <Window.hpp>
#include <Model/Model.hpp>
#include <Utility/Input.hpp>

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
    constexpr int         width      = 1280;
    constexpr int         height     = 720;
    constexpr const char *windowName = "LearnOpenGL";

    try {
        window = createMainWindow(width, height, windowName);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } catch (const std::string &error) {
        std::cerr << "Error creating window:\n" << error << std::endl;
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    Model teapot("teapot/teapot.obj");
    Model backpack("backpack/backpack.obj");
    Model yoda("yoda/yoda.obj");

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

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    defaultShader.setVec3("lightColor", lightColor);

    glm::vec3 ambientColor(0.2f, 0.2f, 0.2f);
    defaultShader.setVec3("ambientColor", ambientColor);

    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    defaultShader.setMat4("model", model);

    Input::Init(window);
    Camera camera(window);

    try {
        while (glfwWindowShouldClose(window) == GL_FALSE) {
            processImput(window);
            camera.update();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT(hicpp-signed-bitwise)

            // if (Input::isKeyPressed(GLFW_KEY_P)) {
            defaultShader.setVec3("lightPos", camera.getPosition());
            // }

            defaultShader.setVec3("viewPos", camera.getPosition());
            defaultShader.setMat4("view", camera.getView());
            defaultShader.setMat4("projection", camera.getProjection());

            backpack.Draw(defaultShader);
            teapot.Draw(defaultShader);
            yoda.Draw(defaultShader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } catch (const std::runtime_error &error) {
        std::cerr << "Error running main loop:\n" << error.what() << std::endl;
        return 1;
    }

    backpack.deleteModel();
    teapot.deleteModel();
    yoda.deleteModel();
    defaultShader.deleteShader();

    glfwTerminate();
    return 0;
}