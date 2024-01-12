#include "Model.hpp"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <format>
#include <unordered_map>
#include <utility>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "fs_helpers.hpp"
#include "Mesh.hpp"

#include <iostream>

void Model::loadModel(const std::string &modelName) {
    std::filesystem::path path = fs_helpers::getPathToModel(modelName);

    directory = path.parent_path();

    Assimp::Importer importer;
    const aiScene   *scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (scene == nullptr || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0u) || (scene->mRootNode) == nullptr) {
        throw std::runtime_error(std::format("Assimp::Importer::ReadFile | {}", importer.GetErrorString()));
    }

    processNode(scene->mRootNode, scene);
}

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
void Model::processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // then do the same for each of its children
    for (size_t i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Mesh::Vertex>  vertices;
    std::vector<unsigned int>  indices;
    std::vector<Mesh::Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Mesh::Vertex vertex{};

        vertex.Position  = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Normal    = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.TexCoords = mesh->mTextureCoords[0] == nullptr
                                   ? glm::vec2(0.0f, 0.0f)
                                   : glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Mesh::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Mesh::Texture> specularMaps =
            loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    return {vertices, indices, textures};
};
// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

std::vector<Mesh::Texture> Model::loadMaterialTextures(aiMaterial        *mat,
                                                       aiTextureType      type,
                                                       const std::string &typeName) {
    std::vector<Mesh::Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString texturePath;
        mat->GetTexture(type, i, &texturePath);

        Mesh::Texture texture{};
        texture.id   = getTextureId(texturePath.C_Str());
        texture.type = typeName;
        texture.path = texturePath.C_Str();

        textures.push_back(texture);
    }
    return textures;
}

GLuint Model::getTextureId(const std::string &texturePath) {
    static std::unordered_map<std::string, GLuint> loadedTextures{};

    std::string path = fs_helpers::isAbsolutePath(texturePath) ? texturePath : (directory / texturePath).string();

    auto iterator = loadedTextures.find(path);

    if (iterator != loadedTextures.end()) {
        return iterator->second;
    }

    int widthTex      = 0;
    int heightTex     = 0;
    int nrChannelsTex = 0;

    stbi_set_flip_vertically_on_load(GL_TRUE);
    unsigned char *dataTexture = stbi_load(path.c_str(), &widthTex, &heightTex, &nrChannelsTex, 0);

    if (dataTexture == nullptr) {
        throw std::runtime_error(std::format("stbi_load | {}", stbi_failure_reason()));
    }

    GLenum format = 0;

    if (nrChannelsTex == 1) {
        format = GL_RED;
    } else if (nrChannelsTex == 3) {
        format = GL_RGB;
    } else if (nrChannelsTex == 4) {
        format = GL_RGBA;
    }

    GLuint textureId = 0;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, widthTex, heightTex, 0, format, GL_UNSIGNED_BYTE, dataTexture);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(dataTexture);

    loadedTextures[path] = textureId;
    return textureId;
}

void Model::Draw(Shader &shader) {
    shader.use();
    for (auto &mesh : meshes) {
        mesh.Draw(shader);
    }
}