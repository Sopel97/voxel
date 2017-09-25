#include "block/BlockResourceLoader.h"

#include "../LibS/Json.h"

std::pair<std::string, std::unique_ptr<BlockFactory>> ResourceLoader<BlockFactory>::load(const std::string& path)
{
    ls::json::Document config = ls::json::Document::fromFile(path);
    const auto& blockConfig = config["block"];

    std::string blockName = blockConfig["name"].getString();

    BlockFactoryFactory* blockFactoryFactory = nullptr;

    std::string typeName = blockConfig["type"].getString();
    try
    {
        blockFactoryFactory = blockFactoryFactories().at(typeName).get();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No block type with name " + typeName);
    }

    std::unique_ptr<BlockFactory> blockFactory = blockFactoryFactory->createBlockFactory(blockConfig);

    Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return
        "Loaded block: " + blockName + " ;with type: " + typeName;
    });

    return std::make_pair(blockName, std::move(blockFactory));
}
