#include "moduleregistry.h"

namespace wake
{
    ModuleAutomation::ModuleAutomation(lua_CFunction openFunc, int priority)
    {
        W_MODULE_REGISTRY.addToRegistry(openFunc, priority);
    }

    ModuleRegistry& ModuleRegistry::get()
    {
        static ModuleRegistry instance;
        return instance;
    }

    ModuleRegistry::ModuleRegistry()
    {
    }

    ModuleRegistry::ModuleRegistry(const ModuleRegistry& other)
    {

    }

    ModuleRegistry& ModuleRegistry::operator=(const ModuleRegistry& other)
    {
        return *this;
    }

    ModuleRegistry::~ModuleRegistry()
    {
    }

    void ModuleRegistry::addToRegistry(lua_CFunction openFunc, int priority)
    {
        modules.push_back(std::make_tuple(priority, openFunc));
    }

    void ModuleRegistry::registerAll(lua_State* L)
    {
        modules.sort([](std::tuple<int, lua_CFunction> a, std::tuple<int, lua_CFunction> b)
                     { return std::get<0>(a) > std::get<0>(b); });
        for (auto entry : modules)
        {
            lua_pushcfunction(L, std::get<1>(entry));
            lua_call(L, 0, 0);
        }
    }
}