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
		if (!window)
		{
			std::cout << "Cannot run engine with uninitialized window (did startup succeed?)" << std::endl;
			return false;
		}

		while (!glfwWindowShouldClose(window))
		{
			TickEvent.call(glfwGetTime());

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		return true;
	}

	Engine::Engine()
    {

    }

    Engine::~Engine()
    {

    }
}