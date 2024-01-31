#include "Shader.hpp"

#include <glad/glad.h>

#include <array>
#include <cstring>
#include <format>
#include <fstream>
#include <string>
#include <stdexcept>

#include <Utility/fs_helpers.hpp>

void Shader::add(const std::string& shaderName, const Shader::Type& type) const {

    if (type == Type::PROGRAM) {
        throw std::runtime_error("Shader::add | Type can't be program");
    }

    std::string   path = fs_helpers::getPathToShader(shaderName).string();
    std::ifstream file;
    file.open(path);

    if (file.fail()) {
        const std::string errorMessage =
                std::format("Shader::add | Failed to open file: {} | System Error: {}", path, std::to_string(errno));
        throw std::runtime_error(errorMessage);
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

    constexpr size_t MAX_LENGTH = 512;
    switch (type) {
    case Shader::Type::PROGRAM:
        glGetProgramiv(shader, GL_LINK_STATUS, &success);

        if (success == GL_FALSE) {
            std::array<GLchar, MAX_LENGTH> infoLog = {};
            glGetProgramInfoLog(shader, sizeof(infoLog), nullptr, infoLog.data());
            throw std::runtime_error(infoLog.data());
        }

        break;
    default:
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (success == GL_FALSE) {
            std::array<GLchar, MAX_LENGTH> infoLog = {};
            glGetShaderInfoLog(shader, infoLog.size(), nullptr, infoLog.data());
            throw std::runtime_error(infoLog.data());
        }
        break;
    }
}