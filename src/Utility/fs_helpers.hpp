#pragma once

#include <string>
#include <filesystem>

namespace fs_helpers {
std::filesystem::path getPathToBinary();
std::filesystem::path getPathToBinaryDirectory();
std::filesystem::path getPathToResourcesDirectory();
std::filesystem::path getPathToShadersDirectory();
std::filesystem::path getPathToTexturesDirectory();
std::filesystem::path getPathToModelsDirectory();

std::filesystem::path getPathToShader(const std::string& shaderName);
std::filesystem::path getPathToTexture(const std::string& textureName);
std::filesystem::path getPathToModel(const std::string& modelName);

bool isAbsolutePath(const std::string& path);
} // namespace fs_helpers