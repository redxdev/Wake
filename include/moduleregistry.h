#pragma once

#include "luautil.h"

#include <list>
#include <tuple>

#define W_MODULE_REGISTRY (wake::ModuleRegistry::get())

#define W_REGISTER_MODULE_PRIORITY(openFunc, priority) static ModuleAutomation __w_module_##openFunc(openFunc, priority)
#define W_REGISTER_MODULE(openFunc) W_REGISTER_MODULE_PRIORITY(openFunc, 0)

namespace wake
{
    class ModuleAutomation
    {
    public:
        ModuleAutomation(lua_CFunction openFunc, int priority);
    };

    class ModuleRegistry
    {
    public:
        static ModuleRegistry& get();

    public:
        void addToRegistry(lua_CFunction openFunc, int priority = 0);

        void registerAll(lua_State* L);

    private:
        ModuleRegistry();
        ModuleRegistry(const ModuleRegistry& other) = delete;

        ModuleRegistry& operator=(const ModuleRegistry& other) = delete;

        ~ModuleRegistry();

        std::list<std::tuple<int, lua_CFunction>> modules;
    };
}