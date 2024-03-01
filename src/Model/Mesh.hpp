#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

#include <Shader.hpp>

class Mesh {
    public:
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Texture {
        unsigned int id;
        std::string  type;
        std::string  path;
    };

    Mesh(std::vector<Vertex>       vertices,
         std::vector<unsigned int> indices,
         std::vector<Texture>      textures,
         float                     shininess);

    void deleteMesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Draw(Shader& shader);

    private:
    // mesh data
    std::vector<Vertex>       m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture>      m_textures;
    float                     m_shininess;

    //  render data
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    void setupMesh();
};