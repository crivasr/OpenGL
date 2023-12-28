#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

#include <iostream>
#include <string>
#include <vector>

#include "Shader.hpp"
#include "Window.hpp"

void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processImput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    GLFWwindow* window;

    try {
        window = createMainWindow(800, 600, "LearnOpenGL");
    } catch (const std::string& error) {
        fprintf(stderr, "Error creating window:\n %s", error.c_str());
        exit(EXIT_FAILURE);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // clang-format off
    float triangle[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };
    // clang-format on

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // unbind VAO
    glBindVertexArray(0);

    Shader shaderProgram;

    try {
        shaderProgram.add("test.vert", Shader::VERTEX);
        shaderProgram.add("test.frag", Shader::FRAGMENT);
        shaderProgram.link();
    } catch (const std::string& error) {
        fprintf(stderr, "Error creating shaders:\n %s\n", error.c_str());
        exit(EXIT_FAILURE);
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    try {
        while (glfwWindowShouldClose(window) == GL_FALSE) {
            processImput(window);

            glClear(GL_COLOR_BUFFER_BIT);

            shaderProgram.use();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } catch (const std::string& error) {
        fprintf(stderr, "Error running main loop:\n %s\n", error.c_str());
        exit(EXIT_FAILURE);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shaderProgram.deleteShader();

    glfwTerminate();
    return 0;
}