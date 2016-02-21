#include "engine.h"

#include <iostream>

namespace wake
{
    static void error_callback(int err, const char* description)
    {
        std::cout << "GLFW error: " << description << std::endl;
    }

    Engine& Engine::get()
    {
        static Engine instance;
        return instance;
    }

    bool Engine::startup()
    {
        glfwSetErrorCallback(&error_callback);
        if (!glfwInit())
        {
            std::cout << "Unable to initialize GLFW." << std::endl;
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        window = glfwCreateWindow(800, 600, "Wake", nullptr, nullptr);
        if (window == nullptr)
        {
            std::cout << "Unable to create window." << std::endl;
            glfwTerminate();

            return false;
        }

        glfwMakeContextCurrent(window);
		glfwSwapInterval(1);

        if (glewInit() != GLEW_OK)
        {
            std::cout << "Unable to initialize GLEW." << std::endl;
            glfwTerminate();

            return false;
        }

        return true;
    }

    bool Engine::shutdown()
    {
        glfwTerminate();
        window = nullptr;

        return true;
    }

	bool Engine::run()
	{
        if (running)
        {
            std::cout << "Cannot run() while already running!" << std::endl;
            return false;
        }

		if (!window)
		{
			std::cout << "Cannot run engine with uninitialized window (did startup succeed?)" << std::endl;
			return false;
		}

        running = true;

        double lastTime = glfwGetTime();

		while (running && !glfwWindowShouldClose(window))
		{
            double frameTime = glfwGetTime() - lastTime;
            lastTime = glfwGetTime();

			TickEvent.call(frameTime);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

        running = false;

        QuitEvent.call();

		return true;
	}

    bool Engine::isRunning() const
    {
        return running;
    }

    void Engine::stop()
    {
        running = false;
    }

    double Engine::getTime() const
    {
        return glfwGetTime();
    }

	Engine::Engine()
    {

    }

    Engine::~Engine()
    {

    }
}