#include "Shader.hpp"

#include <glad/glad.h>

#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef __WIN32__
#include <Windows.h>
#endif

void Shader::add(const std::string& shaderName, const Shader::Type& type) const {

    if (type == Type::PROGRAM) {
        throw std::string("Shader::add | Type can't be program");
    }

#ifdef __WIN32__
    char pathBuffer[MAX_PATH];
    GetModuleFileName(nullptr, pathBuffer, MAX_PATH);
    std::filesystem::path path(pathBuffer); // TODO: Check if this works on windows
    std::string           pathToDirectory = path.parent_path().string();
#else
    const std::string pathToDirectory = std::filesystem::canonical("/proc/self/exe").parent_path().string();
#endif

    std::string   fullPath = pathToDirectory + "/resources/shaders/" + shaderName;
    std::ifstream file;
    file.open(fullPath);

    if (file.fail()) {
        std::string errorMessage =
                std::format("Shader::add | Failed to open file: {} | System Error: {}", fullPath, std::strerror(errno));
        throw errorMessage;
    }

    std::stringstream sourceBuffer;
    sourceBuffer << file.rdbuf();
    file.close();

    const std::string sourceString = sourceBuffer.str();

    const GLuint shader = glCreateShader(type);

    const GLchar* source = sourceString.c_str();
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    Shader::checkCompileErrors(shader, type);

    glAttachShader(m_programID, shader);
    glDeleteShader(shader);
}

void Shader::checkCompileErrors(GLuint shader, Shader::Type type) {
    GLint success = 0;
    switch (type) {
    case Shader::Type::PROGRAM:
        glGetProgramiv(shader, GL_LINK_STATUS, &success);

        if (success == GL_FALSE) {
            GLchar infoLog[512];
            glGetProgramInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            throw std::string(infoLog);
        }

        break;
    default:
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (success == GL_FALSE) {
            GLchar infoLog[512];
            glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            throw std::string(infoLog);
        }
        break;
    }
}