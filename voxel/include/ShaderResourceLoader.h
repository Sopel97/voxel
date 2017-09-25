#pragma once

#include "ResourceManager.h"

#include "../LibS/OpenGL/Shader.h"

template <>
class ResourceLoader<ls::gl::ShaderProgram>
{
public:
    static std::pair<std::string, std::unique_ptr<ls::gl::ShaderProgram>> load(const std::string& vertexPath, const std::string& fragmentPath); //should return nullptr when resource was not loaded
};