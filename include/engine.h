#pragma once

#include "glutil.h"

#define W_ENGINE (wake::Engine::get())

namespace wake
{
    class Engine
    {
    public:
        static Engine& get();

    public:
        bool startup();

        bool shutdown();

    private:
        Engine();

        ~Engine();

        GLFWwindow* window = nullptr;
    };
}