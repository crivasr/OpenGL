#pragma once

#include <glad/glad.h>

#include <string>
#include <stdexcept>

class Shader {
    private:
    GLuint m_programID;
    bool   m_linked = false;

    public:
    enum Type { VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER, PROGRAM = GL_PROGRAM };

    Shader() noexcept : m_programID(glCreateProgram()) {}
    // We don't automatically delete the program, because it will crash if glfgTerminate() is called before
    ~Shader() = default; 

    // Declare copy constructor, copy assignment operator, move constructor, and move assignment operator as
    // deleted
    Shader(const Shader &)            = delete;
    Shader &operator=(const Shader &) = delete;
    Shader(Shader &&)                 = delete;
    Shader &operator=(Shader &&)      = delete;

    void add(const std::string &shaderName, const Type &type) const;
    void link() {
        m_linked = true;
        glLinkProgram(m_programID);
        Shader::checkCompileErrors(m_programID, Shader::Type::PROGRAM);
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
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
    }
};