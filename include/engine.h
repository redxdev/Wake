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
        Event<> QuitEvent;

        bool startup();

        bool shutdown();

		bool run();

        bool isRunning() const;

        void stop();

        double getTime() const;

        void setClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a);

    private:
        Engine();
        Engine(const Engine& other);
        Engine& operator=(const Engine& other);

        ~Engine();

        GLFWwindow* window = nullptr;

        bool running = false;
    };
}