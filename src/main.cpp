#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>
#include <iostream>
#include <memory>

#include "Shader.hpp"
#include "Window.hpp"

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
    } catch (const std::string &error) {
        std::cerr << "Error creating window:\n" << error << std::endl;
        return 1;
    }

    constexpr int triangleSize = 18;

    // clang-format off
    std::array<float, triangleSize> triangle({
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right // NOLINT
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left  // NOLINT
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top          // NOLINT
    });
    // clang-format on

    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    const int positionIndex = 0;
    const int colorIndex    = 1;

    const int positionSize = 3;
    const int colorSize    = 3;

    const int strideSize = (positionSize + colorSize) * sizeof(float);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
    const void *colorOffset = reinterpret_cast<void *>(positionSize * sizeof(float));

    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(positionIndex, positionSize, GL_FLOAT, GL_FALSE, strideSize, nullptr);
    glEnableVertexAttribArray(positionIndex);

    glVertexAttribPointer(colorIndex, colorSize, GL_FLOAT, GL_FALSE, strideSize, colorOffset);
    glEnableVertexAttribArray(colorIndex);

    // unbind VAO
    glBindVertexArray(0);

    Shader shaderProgram;

    try {
        shaderProgram.add("test.vert", Shader::VERTEX);
        shaderProgram.add("test.frag", Shader::FRAGMENT);
        shaderProgram.link();
    } catch (const std::runtime_error &error) {
        std::cerr << "Error creating shaders:\n" << error.what() << std::endl;
        return 1;
    }

    const float clearColorRed   = 0.2f;
    const float clearColorGreen = 0.3f;
    const float clearColorBlue  = 0.3f;
    const float clearColorAlpha = 1.0f;

    glClearColor(clearColorRed, clearColorGreen, clearColorBlue, clearColorAlpha);

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
    } catch (const std::runtime_error &error) {
        std::cerr << "Error running main loop:\n" << error.what() << std::endl;
        return 1;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram.getProgramID());

    glfwTerminate();
    return 0;
}