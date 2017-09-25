#pragma once

#include "ResourceManager.h"

#include "Spritesheet.h"

template <>
class ResourceLoader<Spritesheet>
{
public:
    static std::pair<std::string, std::unique_ptr<Spritesheet>> load(const std::string& path, const ls::Vec2I& gridSize, const ls::Vec2I& padding, bool repeated); //should return nullptr when resource was not loaded
};