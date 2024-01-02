#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <array>
#include <iostream>
#include <memory>
#include <filesystem>

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

    // clang-format off
    std::array<float, 32> triangle({
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top righ
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    });

    std::array<unsigned int, 6> indices({
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    });
    // clang-format on

    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle.data(), GL_STATIC_DRAW);

    GLuint EBO = 0;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

    const int positionIndex = 0;
    const int colorIndex    = 1;
    const int textureIndex  = 2;

    const int positionSize = 3;
    const int colorSize    = 3;
    const int textureSize  = 2;

    const int strideSize = (positionSize + colorSize + textureSize) * sizeof(float);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
    const void *colorOffset = reinterpret_cast<void *>(positionSize * sizeof(float));
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
    const void *textureOffset = reinterpret_cast<void *>((positionSize + colorSize) * sizeof(float));

    glVertexAttribPointer(positionIndex, positionSize, GL_FLOAT, GL_FALSE, strideSize, nullptr);
    glEnableVertexAttribArray(positionIndex);

    glVertexAttribPointer(colorIndex, colorSize, GL_FLOAT, GL_FALSE, strideSize, colorOffset);
    glEnableVertexAttribArray(colorIndex);

    glVertexAttribPointer(textureIndex, textureSize, GL_FLOAT, GL_FALSE, strideSize, textureOffset);
    glEnableVertexAttribArray(textureIndex);

    int widthTex1, heightTex1, nrChannelsTex1; // NOLINT
    int widthTex2, heightTex2, nrChannelsTex2; // NOLINT

    const std::string pathToDirectory = std::filesystem::canonical("/proc/self/exe").parent_path().string();
    std::string       pathTexture1    = pathToDirectory + "/resources/textures/container.jpg";
    std::string       pathTexture2    = pathToDirectory + "/resources/textures/awesomeface.png";

    stbi_set_flip_vertically_on_load(GL_TRUE);
    unsigned char *dataTexture1 = stbi_load(pathTexture1.c_str(), &widthTex1, &heightTex1, &nrChannelsTex1, 0);
    unsigned char *dataTexture2 = stbi_load(pathTexture2.c_str(), &widthTex2, &heightTex2, &nrChannelsTex2, 0);

    if (dataTexture1 == nullptr || dataTexture2 == nullptr) {
        std::cerr << "Failed to load texture" << std::endl;
        return 1;
    }

    std::array<GLuint, 2> textures{};
    glGenTextures(2, textures.data());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures.at(0));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex1, heightTex1, 0, GL_RGB, GL_UNSIGNED_BYTE, dataTexture1);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(dataTexture1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures.at(1));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex2, heightTex2, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataTexture2);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(dataTexture2);

    Shader shaderProgram;

    try {
        shaderProgram.add("test.vert", Shader::VERTEX);
        shaderProgram.add("test.frag", Shader::FRAGMENT);
        shaderProgram.link();
    } catch (const std::runtime_error &error) {
        std::cerr << "Error creating shaders:\n" << error.what() << std::endl;
        return 1;
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    shaderProgram.use();
    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);

    try {
        while (glfwWindowShouldClose(window) == GL_FALSE) {
            processImput(window);

            glClear(GL_COLOR_BUFFER_BIT);

            shaderProgram.use();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } catch (const std::runtime_error &error) {
        std::cerr << "Error running main loop:\n" << error.what() << std::endl;
        return 1;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram.getProgramID());

    glfwTerminate();
    return 0;
}