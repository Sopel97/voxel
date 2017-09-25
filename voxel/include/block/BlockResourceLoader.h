#pragma once

#include "ResourceManager.h"
#include "Logger.h"

#include "BlockFactory.h"
#include "BlockFactoryFactory.h"

#include <memory>
#include <map>
#include <utility>

#define REGISTER_BLOCK_TYPE(TYPE) \
    namespace ___BlockTypeRegistering \
    { \
        const ResourceLoader<BlockFactory>::BlockTypeRegistrar<TYPE> TYPE ## _block_var (#TYPE); \
    }

template <>
class ResourceLoader<BlockFactory>
{
public:

    template<class T>
    struct BlockTypeRegistrar
    {
        BlockTypeRegistrar(const std::string& name)
        {
            Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return
                "Registered block type: " + name;
            });
            ResourceLoader<BlockFactory>::blockFactoryFactories().insert(std::make_pair(name, std::make_unique<SpecificBlockFactoryFactory<T>>()));
        }
    };

    static std::pair<std::string, std::unique_ptr<BlockFactory>> load(const std::string& path); //should return nullptr when resource was not loaded
protected:

    static std::map<std::string, std::unique_ptr<BlockFactoryFactory>>& blockFactoryFactories()
    {
        static std::map<std::string, std::unique_ptr<BlockFactoryFactory>> _blockFactoryFactories;
        return _blockFactoryFactories;
    }

};