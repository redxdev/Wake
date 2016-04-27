#include "engine.h"

#include <iostream>
#include <glm/glm.hpp>

// REMOVE ME
#include "mesh.h"

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
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

        W_GL_CHECK();

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Unable to initialize GLEW." << std::endl;
            glfwTerminate();

            return false;
        }

        W_GL_CHECK();

        glViewport(0, 0, 800, 600); // TODO: Window size

        setClearColor(0.f, 0.f, 0.f, 1.f);

        W_GL_CHECK();

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

        // <  REMOVE ME >
        std::vector<Vertex> vertices = {
                Vertex(glm::vec3(0, 0.5f, 0)),
                Vertex(glm::vec3(0.5f, -0.5f, 0)),
                Vertex(glm::vec3(-0.5f, -0.5f, 0))
        };

        Mesh mesh(vertices);
        // </ REMOVE ME >

		while (running && !glfwWindowShouldClose(window))
		{
            double frameTime = glfwGetTime() - lastTime;
            lastTime = glfwGetTime();

            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT);

			TickEvent.call(frameTime);

            mesh.draw(); // REMOVE ME

            glfwSwapBuffers(window);
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

    void Engine::setClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
    {
        glClearColor(r, g, b, a);
    }

	Engine::Engine()
    {

    }

    Engine::Engine(const Engine& other)
    {

    }

    Engine& Engine::operator=(const Engine& other)
    {
        return *this;
    }

    Engine::~Engine()
    {

    }
}
