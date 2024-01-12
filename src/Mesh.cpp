#include "Mesh.hpp"
#include "glad/glad.h"
#include <string>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
        : vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures)) {
    setupMesh();
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // GLsizeiptr is a signed, if there are too many vertices or indices, it will overflow and result in a negative size
    const GLsizeiptr verticesSize = vertices.size() * sizeof(Vertex);
    const GLsizeiptr indicesSize  = indices.size() * sizeof(unsigned int);

    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices.data(), GL_STATIC_DRAW);

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

constexpr std::string DIFFUSE = "texture_diffuse";

void Mesh::Draw(Shader &shader) {
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++) {
        const auto texture = textures[i];

        glActiveTexture(GL_TEXTURE0 + i);

        std::string number = texture.type == DIFFUSE ? std::to_string(diffuseNr++) : std::to_string(specularNr++);

        shader.setUInt("material." + texture.type + number, i);
        glBindTexture(GL_TEXTURE_2D, texture.id);
    }

    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}