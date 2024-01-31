#pragma once

#include <Utility/OpenGlHeaders.hpp>
#include <glm/glm.hpp>

#include <string>
#include <stdexcept>

class Shader {
    private:
    GLuint m_programID;
    bool   m_linked = false;

    public:
    enum Type { VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER, PROGRAM = GL_PROGRAM };

    Shader() noexcept : m_programID(glCreateProgram()) {}
    void deleteShader() const { glDeleteProgram(m_programID); }

    void add(const std::string &shaderName, const Type &type) const;
    void link() {
        m_linked = true;
        glLinkProgram(m_programID);
        Shader::checkCompileErrors(m_programID, Type::PROGRAM);
    }

    void use() const {
        if (!m_linked) {
            throw std::runtime_error("Shader::use | Program not yet linked, can't be used");
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
    void setUInt(const std::string &name, unsigned int value) const {
        glUniform1ui(glGetUniformLocation(m_programID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
    }
    void setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(m_programID, name.c_str()), 1, &value[0]);
    }
    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};