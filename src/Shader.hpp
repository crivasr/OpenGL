#pragma once

#include <glad/glad.h>

#include <cstddef>
#include <string>
#include <vector>

class Shader {
    private:
    GLuint m_programID;
    bool   m_deleted = false;
    bool   m_linked  = false;

    public:
    enum Type { VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER, PROGRAM = GL_PROGRAM };

    Shader() noexcept : m_programID(glCreateProgram()) {}

    void deleteShader() noexcept {
        m_deleted = true;
        glUseProgram(0);
        glDeleteProgram(m_programID);
    }

    void add(const std::string &shaderName, const Type &type) const;
    void link() {
        m_linked = true;
        glLinkProgram(m_programID);
        Shader::checkCompileErrors(m_programID, Shader::Type::PROGRAM);
    }

    void use() const {
        if (m_deleted) {
            throw std::string("Shader::use | Program deleted, can't be used");
        }

        if (!m_linked) {
            throw std::string("Shader::use | Program not yet linked, can't be used");
        }

        glUseProgram(m_programID);
    }

    [[nodiscard]] GLuint getProgramID() const noexcept { return m_programID; }

    static void checkCompileErrors(GLuint shader, Type type);

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(m_programID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
    }
};