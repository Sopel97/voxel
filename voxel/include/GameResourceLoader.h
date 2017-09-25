#pragma once

#include <vector>
#include <string>

class GameResourceLoader
{
public:
    static void loadAssets();
    static void loadTextures();
    static void loadBlocks();
    static void loadShaders();

private:
    static bool m_areAssetsLoaded;

    static std::wstring stringToWString(const std::string &s);
    static std::string wstringToString(const std::wstring &s);
    static std::vector<std::string> scanForFiles(const std::string& path, const std::string& query);
};
