#include "fs_helpers.hpp"

#include <filesystem>
#include <string>

#ifdef __WIN32__
#include <Windows.h>
#endif

namespace fs_helpers {

std::filesystem::path getPathToBinary() {
#ifdef __WIN32__
    char pathBuffer[MAX_PATH];
    GetModuleFileName(nullptr, pathBuffer, MAX_PATH);
    std::filesystem::path path(pathBuffer);
#else
    const std::string path = std::filesystem::canonical("/proc/self/exe");
#endif
    return path;
}

std::filesystem::path getPathToBinaryDirectory() {
    return getPathToBinary().parent_path();
}

std::filesystem::path getPathToResourcesDirectory() {
    return getPathToBinaryDirectory() / "resources";
}

std::filesystem::path getPathToShadersDirectory() {
    return getPathToResourcesDirectory() / "shaders";
}

std::filesystem::path getPathToTexturesDirectory() {
    return getPathToResourcesDirectory() / "textures";
}

std::filesystem::path getPathToModelsDirectory() {
    return getPathToResourcesDirectory() / "models";
}

std::filesystem::path getPathToShader(const std::string& shaderName) {
    return shaderName.starts_with("/") ? std::filesystem::path(shaderName) : getPathToShadersDirectory() / shaderName;
}

std::filesystem::path getPathToTexture(const std::string& textureName) {
    return textureName.starts_with("/") ? std::filesystem::path(textureName)
                                        : getPathToTexturesDirectory() / textureName;
}

std::filesystem::path getPathToModel(const std::string& modelName) {
    return modelName.starts_with("/") ? std::filesystem::path(modelName) : getPathToModelsDirectory() / modelName;
}

bool isAbsolutePath(const std::string& path) {
    return path.starts_with("/");
}

} // namespace fs_helpers