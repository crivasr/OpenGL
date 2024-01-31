#pragma once

#include <assimp/scene.h>

#include <string>
#include <vector>
#include <filesystem>

#include "Mesh.hpp"
#include <Shader.hpp>

class Model {
    public:
    explicit Model(const std::string &modelName) { loadModel(modelName); }

    void deleteModel() {
        for (auto &mesh : meshes) {
            mesh.deleteMesh();
        }
    }

    void Draw(Shader &shader);

    private:
    // model data
    std::vector<Mesh>     meshes;
    std::filesystem::path directory{};

    void   loadModel(const std::string &modelName);
    void   processNode(aiNode *node, const aiScene *scene);
    Mesh   processMesh(aiMesh *mesh, const aiScene *scene);
    GLuint getTextureId(const std::string &texturePath);

    std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);
};