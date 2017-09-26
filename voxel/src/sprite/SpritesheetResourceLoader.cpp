#include "sprite/SpritesheetResourceLoader.h"

#include "Logger.h"

std::pair<std::string, std::unique_ptr<Spritesheet>> ResourceLoader<Spritesheet>::load(const std::string& path, const ls::Vec2I& gridSize, const ls::Vec2I& padding, bool repeated)
{
    std::unique_ptr<Spritesheet> spritesheet = std::make_unique<Spritesheet>(path, gridSize, padding);
    spritesheet->setRepeated(repeated);    
    spritesheet->texture().setMagFilter(GL_NEAREST);
    spritesheet->texture().setMinFilter(GL_NEAREST);

    Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return
        "Loaded texture: " + path +
        ";gridSize: (" + std::to_string(gridSize.x) + ", " + std::to_string(gridSize.y) + ") " +
        ";padding: " + std::to_string(padding.x) + ", " + std::to_string(padding.y) + ") " +
        ";repeated: " + std::to_string(repeated);
    });

    return std::make_pair(path, std::move(spritesheet));
}