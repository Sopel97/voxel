#include "GameResourceLoader.h"

#include "ResourceManager.h"
#include "sprite/SpritesheetResourceLoader.h"
#include "block/BlockResourceLoader.h"
#include "ShaderResourceLoader.h"

#include "../LibS/Shapes/Vec2.h"
#include "../LibS/OpenGL/Shader.h"

#include "../LibS/Json.h"

#define NOMINMAX
#include <windows.h>

bool GameResourceLoader::m_areAssetsLoaded = false;

void GameResourceLoader::loadAssets()
{
    if (m_areAssetsLoaded) return;

    loadTextures();
    loadShaders();
    loadBlocks();

    m_areAssetsLoaded = true;
}
void GameResourceLoader::loadTextures()
{
    ls::json::Document config = ls::json::Document::fromFile("assets/textures/textures.json");
    const auto& textureList = config["textures"];

    const int numEntries = textureList.size();
    for (int i = 0; i < numEntries; ++i)
    {
        const std::string path = textureList[i]["path"].getString();
        const std::string name = textureList[i]["name"].getString();
        const ls::Vec2I gridSize(
            static_cast<int>(textureList[i]["gridSize"][0].getIntOr(1)),
            static_cast<int>(textureList[i]["gridSize"][1].getIntOr(1))
        );
        const ls::Vec2I padding(
            static_cast<int>(textureList[i]["padding"][0].getIntOr(0)),
            static_cast<int>(textureList[i]["padding"][1].getIntOr(0))
        );
        const bool isRepeated = textureList[i]["repeated"].getBoolOr(false);
        ResourceManager<Spritesheet>::instance().loadWithName(name, std::string("assets/textures/") + path, gridSize, padding, isRepeated);
    }
}
void GameResourceLoader::loadBlocks()
{
    for (const auto& tilePath : scanForFiles("assets/blocks/", "*.json"))
    {
        ResourceManager<BlockFactory>::instance().load(tilePath);
    }
}
void GameResourceLoader::loadShaders()
{
    ls::json::Document config = ls::json::Document::fromFile("assets/shaders/shaders.json");
    const auto& shaderList = config["shaders"];

    const int numEntries = shaderList.size();
    for (int i = 0; i < numEntries; ++i)
    {
        const std::string name = shaderList[i]["name"].getString();
        const std::string vertexPath = shaderList[i]["vertex"].getString();
        const std::string fragmentPath = shaderList[i]["fragment"].getString();
        ResourceManager<ls::gl::ShaderProgram>::instance().loadWithName(name, std::string("assets/shaders/") + vertexPath, std::string("assets/shaders/") + fragmentPath);
    }
}
std::wstring GameResourceLoader::stringToWString(const std::string &s)
{
    return std::wstring(s.begin(), s.end());
}
std::string GameResourceLoader::wstringToString(const std::wstring &s)
{
    return std::string(s.begin(), s.end());
}
std::vector<std::string> GameResourceLoader::scanForFiles(const std::string& path, const std::string& query)
{
    std::vector<std::string> files;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    std::wstring fullPath = stringToWString(path + query);
    hFind = FindFirstFile(fullPath.c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        files.push_back(path + wstringToString(findFileData.cFileName));
    }
    while (FindNextFile(hFind, &findFileData))
    {
        files.push_back(path + wstringToString(findFileData.cFileName));
    }
    FindClose(hFind);
    return files;
}
