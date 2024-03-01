#include "Mesh.hpp"

#include "Texture.hpp"

#include <Utility/OpenGlHeaders.hpp>

#include <vector>
#include <string>
#include <iostream>

Mesh::Mesh(std::vector<Vertex>       vertices,
           std::vector<unsigned int> indices,
           std::vector<Texture>      textures,
           float                     shininess)
        : m_vertices(std::move(vertices)),
          m_indices(std::move(indices)),
          m_textures(std::move(textures)),
          m_shininess(shininess) {
    setupMesh();
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // GLsizeiptr is a signed, if there are too many vertices or indices, it will overflow and result in a negative size
    const GLsizeiptr verticesSize = m_vertices.size() * sizeof(Vertex);
    const GLsizeiptr indicesSize  = m_indices.size() * sizeof(unsigned int);

    glBufferData(GL_ARRAY_BUFFER, verticesSize, m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, m_indices.data(), GL_STATIC_DRAW);

    const unsigned int positionIndex = 0;
    const unsigned int normalIndex   = 1;
    const unsigned int texCoordIndex = 2;

    const unsigned int sizeofPosition = 3;
    const unsigned int sizeofNormal   = 3;
    const unsigned int sizeofTexCoord = 2;

    const auto *positionOffset = reinterpret_cast<void *>(offsetof(Vertex, Position));  // NOLINT
    const auto *normalOffset   = reinterpret_cast<void *>(offsetof(Vertex, Normal));    // NOLINT
    const auto *texCoordOffset = reinterpret_cast<void *>(offsetof(Vertex, TexCoords)); // NOLINT

    // vertex positions
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, sizeofPosition, GL_FLOAT, GL_FALSE, sizeof(Vertex), positionOffset);

    // vertex normals
    glEnableVertexAttribArray(normalIndex);
    glVertexAttribPointer(normalIndex, sizeofNormal, GL_FLOAT, GL_FALSE, sizeof(Vertex), normalOffset);

    // vertex texture coords
    glEnableVertexAttribArray(texCoordIndex);
    glVertexAttribPointer(texCoordIndex, sizeofTexCoord, GL_FLOAT, GL_FALSE, sizeof(Vertex), texCoordOffset);

    glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader) {
    unsigned int diffuseNr   = 1;
    unsigned int specularNr  = 1;
    unsigned int roughnessNr = 1;

    // BUG: If the mesh doesn't have one of the texture types,
    // the shader will use the last texture of that type
    for (unsigned int i = 0; i < m_textures.size(); i++) {
        const auto texture = m_textures[i];

        glActiveTexture(GL_TEXTURE0 + i);

        std::string number = texture.type == ::Texture::DIFFUSE     ? std::to_string(diffuseNr++)
                             : texture.type == ::Texture::SPECULAR  ? std::to_string(specularNr++)
                             : texture.type == ::Texture::ROUGHNESS ? std::to_string(roughnessNr++)
                                                                    : "";

        shader.setInt(texture.type + number, (int)i);
        glBindTexture(GL_TEXTURE_2D, texture.id);
    }

    shader.setFloat("material_shininess", m_shininess);

    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}