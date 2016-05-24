#pragma once

#include "event.h"
#include "luautil.h"
#include "pushvalue.h"
#include "scriptmanager.h"

#include <iostream>

#define W_MT_EVENT ("wake.NativeEvent")

namespace wake
{
    namespace binding
    {
        template<typename... Arguments>
        class LuaDelegate : public Delegate<Arguments...>
        {
        private:
            static const std::size_t argCount = sizeof...(Arguments);

        public:
            LuaDelegate(lua_State* L)
            {
                this->L = L;
                if (!lua_isfunction(L, -1))
                {
                    luaL_error(L, "top of stack must be a function");
                }

                funcRef = luaL_ref(L, LUA_REGISTRYINDEX);
            }

            virtual ~LuaDelegate()
            {
                if (W_SCRIPT.isActive())
                    luaL_unref(L, LUA_REGISTRYINDEX, funcRef);
            }

            virtual void call(Arguments... args) override
            {
                lua_rawgeti(L, LUA_REGISTRYINDEX, funcRef);
                pushValue(L, args...);
                if (lua_pcall(L, argCount, 0, 0) != 0)
                {
                    std::cout << "Unable to call lua function from event: " << lua_tostring(L, -1) << std::endl;
                }
            }

            virtual bool equals(Delegate<Arguments...>* other) const override
            {
                auto otherPtr = dynamic_cast<LuaDelegate<Arguments...>*>(other);
                if (!otherPtr)
                    return false;

                lua_rawgeti(L, LUA_REGISTRYINDEX, funcRef);
                lua_rawgeti(L, LUA_REGISTRYINDEX, otherPtr->funcRef);
                bool result = lua_equal(L, -1, -2) != 0;
                lua_pop(L, 2);

                return result;
            }

        private:
            lua_State* L;
            int funcRef;
        };

        template<typename... Arguments>
        class LuaTableDelegate : public Delegate<Arguments...>
        {
        private:
            static const std::size_t argCount = sizeof...(Arguments);

        public:
            LuaTableDelegate(lua_State* L)
            {
                this->L = L;
                if (!lua_isfunction(L, -1))
                {
                    luaL_error(L, "second argument must be a function");
                }

                funcRef = luaL_ref(L, LUA_REGISTRYINDEX);

                if (!lua_istable(L, -1))
                {
                    luaL_error(L, "first argument must be a table");
                }

                tableRef = luaL_ref(L, LUA_REGISTRYINDEX);
            }

            virtual ~LuaTableDelegate()
            {

                if (W_SCRIPT.isActive())
                {
                    luaL_unref(L, LUA_REGISTRYINDEX, funcRef);
                    luaL_unref(L, LUA_REGISTRYINDEX, tableRef);
                }
            }

            virtual void call(Arguments... args) override
            {
                lua_rawgeti(L, LUA_REGISTRYINDEX, funcRef);
                lua_rawgeti(L, LUA_REGISTRYINDEX, tableRef);
                pushValue(L, args...);
                if (lua_pcall(L, argCount + 1, 0, 0) != 0)
                {
                    std::cout << "Unable to call lua function from event: " << lua_tostring(L, -1) << std::endl;
                }
            }

            virtual bool equals(Delegate<Arguments...>* other) const override
            {
                auto otherPtr = dynamic_cast<LuaTableDelegate<Arguments...>*>(other);
                if (!otherPtr)
                    return false;

                lua_rawgeti(L, LUA_REGISTRYINDEX, funcRef);
                lua_rawgeti(L, LUA_REGISTRYINDEX, otherPtr->funcRef);
                bool result = lua_equal(L, -1, -2) != 0;
                lua_pop(L, 2);

                if (!result)
                    return false;

                lua_rawgeti(L, LUA_REGISTRYINDEX, tableRef);
                lua_rawgeti(L, LUA_REGISTRYINDEX, otherPtr->tableRef);
                result = lua_equal(L, -1, -2) != 0;
                lua_pop(L, 2);

                return result;
            }

        private:
            lua_State* L;
            int tableRef;
            int funcRef;
        };

        class ILuaEventWrapper
        {
        public:
            virtual ~ILuaEventWrapper()
            { }

            virtual void bind(lua_State* L) = 0;

            virtual void bindTable(lua_State* L) = 0;

            virtual void unbind(lua_State* L) = 0;

            virtual void unbindTable(lua_State* L) = 0;

            virtual void clear() = 0;
        };

        template<typename... Arguments>
        class LuaEventWrapper : public ILuaEventWrapper
        {
        public:
            typedef Event<Arguments...> EventType;

        public:
            LuaEventWrapper(EventType& event)
                    : event(event)
            {
            }

            virtual ~LuaEventWrapper()
            { }

            virtual void bind(lua_State* L) override
            {
                event.bind(new LuaDelegate<Arguments...>(L));
            }

            virtual void bindTable(lua_State* L) override
            {
                event.bind(new LuaTableDelegate<Arguments...>(L));
            }

            virtual void unbind(lua_State* L) override
            {
                event.unbind(new LuaDelegate<Arguments...>(L));
            }

            virtual void unbindTable(lua_State* L) override
            {
                event.unbind(new LuaTableDelegate<Arguments...>(L));
            }

            virtual void clear() override
            {
                event.clear();
            }

        private:
            EventType& event;
        };

        struct EventContainer
        {
            ILuaEventWrapper* wrapper;
        };

        int luaopen_event(lua_State* L);
    }

    binding::ILuaEventWrapper* luaW_checkevent(lua_State* L, int narg);

    template<typename... T>
    void pushValue(lua_State* L, Event<T...>& event)
    {
        auto container = (binding::EventContainer*) lua_newuserdata(L, sizeof(binding::EventContainer));
        container->wrapper = new binding::LuaEventWrapper<T...>(event);

        luaL_getmetatable(L, W_MT_EVENT);
        lua_setmetatable(L, -2);
    }
}