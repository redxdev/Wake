#pragma once

#include "glutil.h"
#include "event.h"

#define W_ENGINE (wake::Engine::get())

namespace wake
{
    class Engine
    {
    public:
        static Engine& get();

    public:
		Event<double> TickEvent;

        bool startup();

        bool shutdown();

		bool run();

    private:
        Engine();

        ~Engine();

        GLFWwindow* window = nullptr;
    };
}