#include "Shader.hpp"

#include <glad/glad.h>

#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <string>
#include <stdexcept>

#ifdef __WIN32__
#include <Windows.h>
#endif

void Shader::add(const std::string& shaderName, const Shader::Type& type) const {

    if (type == Type::PROGRAM) {
        throw std::runtime_error("Shader::add | Type can't be program");
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
        const std::string errorMessage = std::format(
                "Shader::add | Failed to open file: {} | System Error: {}", fullPath, std::to_string(errno));
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